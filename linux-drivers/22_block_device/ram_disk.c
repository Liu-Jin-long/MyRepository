#include <linux/types.h>
#include <linux/delay.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/ide.h>
#include <linux/uaccess.h>
#include <linux/io.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_irq.h>
#include <linux/irq.h>
#include <linux/of_gpio.h>
#include <linux/gpio.h>
#include <asm/atomic.h>
#include <linux/input.h>
#include <linux/i2c.h>
#include <linux/input/mt.h>
#include <linux/input/touchscreen.h>
#include <asm/unaligned.h>
#include <linux/blkdev.h>
#include <linux/hdreg.h>
#define MAKE_REQUEST 1

#define RAM_DISK_SIZE (2 * 1024 * 1024) // RAM模拟磁盘容量大小2MB
#define RAM_DISK_NAME "ram_disk"
#define RAM_DISK_PARTITIONS 3 // 磁盘分区数目

struct RAM_DISK
{
    int major;
    unsigned char *ram_disk_buf; // ram内存空间,用于模拟块设备
    spinlock_t spinlock;
    struct gendisk *gendisk;
    struct request_queue *queue; // IO请求队列
};
struct RAM_DISK ram_disk;
int ram_disk_open(struct block_device *dev, fmode_t mode)
{
    printk("ram_disk_open\n");
    return 0;
}
void ram_disk_release(struct gendisk *dev, fmode_t mode)
{
    printk("ram_disk_release\n");
}
// 设置机械硬盘的几何结构数据
int ram_disk_getgeo(struct block_device *dev, struct hd_geometry *geo)
{
    geo->heads = 2;                                /* 磁头 */
    geo->cylinders = 32;                           /* 柱面 */
    geo->sectors = RAM_DISK_SIZE / (2 * 32 * 512); /* 磁道上的扇区数量 */
    return 0;
}
struct block_device_operations ram_disk_fops =
    {
        .owner = THIS_MODULE,
        .open = ram_disk_open,
        .release = ram_disk_release,
        .getgeo = ram_disk_getgeo,
};
#if MAKE_REQUEST
void ramdisk_make_request_fn(struct request_queue *q, struct bio *bio)
{
    int offset;
    struct bio_vec bvec;
    struct bvec_iter iter;
    unsigned long len;
    offset = (bio->bi_iter.bi_sector) << 9; // 扇区地址左移9位转换为字节地址
    // 处理bio中的每个段
    bio_for_each_segment(bvec, bio, iter)
    {
        char *ptr = page_address(bvec.bv_page) + bvec.bv_offset; // 内存地址
        len = bvec.bv_len;
        if (bio_data_dir(bio) == READ)
        {
            memcpy(ptr, ram_disk.ram_disk_buf + offset, len);
        }
        else if (bio_data_dir(bio) == WRITE)
        {
            memcpy(ram_disk.ram_disk_buf + offset, ptr, len);
        }
        offset += len;
    }
    set_bit(BIO_UPTODATE, &bio->bi_flags); // IO完成标志
    bio_endio(bio, 0);
}
#else
static void ram_disk_transfer(struct request *req)
{
    unsigned long start = blk_rq_pos(req) << 9; // 扇区地址左移9位转换为字节地址
    unsigned long len = blk_rq_cur_bytes(req);
    void *buffer = bio_data(req->bio);
    if (rq_data_dir(req) == READ) // 读
    {
        memcpy(buffer, ram_disk.ram_disk_buf + start, len);
    }
    else // 写
    {
        memcpy(ram_disk.ram_disk_buf + start, buffer, len);
    }
}
void ram_disk_request_fn(struct request_queue *q)
{
    int err = 0;
    struct request *req;
    // 循环处理请求队列中的每个请求
    req = blk_fetch_request(q);
    while (req)
    {
        // 数据传输
        ram_disk_transfer(req);
        if (!__blk_end_request_cur(req, err))
        {
            req = blk_fetch_request(q);
        }
    }
}
#endif
static int __init RAM_DISK_init(void)
{
    int ret;
    printk("RAM_DISK_init\n");
    ram_disk.ram_disk_buf = kzalloc(RAM_DISK_SIZE, GFP_KERNEL);
    if (ram_disk.ram_disk_buf == NULL)
    {
        ret = -EINVAL;
        goto alloc_buf_failed;
    }
    spin_lock_init(&ram_disk.spinlock);
    // 注册块设备
    ram_disk.major = register_blkdev(0, RAM_DISK_NAME); // 动态分配
    if (ram_disk.major < 0)
    {
        ret = ram_disk.major;
        goto register_blkdev_failed;
    }
    // 分配并初始化gendisk
    ram_disk.gendisk = alloc_disk(RAM_DISK_PARTITIONS);
    if (ram_disk.gendisk == NULL)
    {
        ret = -EINVAL;
        goto gendisk_alloc_failed;
    }
#if MAKE_REQUEST
    // 分配请求队列
    ram_disk.queue = blk_alloc_queue(GFP_KERNEL);
    // 设置“制造请求”函数
    blk_queue_make_request(ram_disk.queue, ramdisk_make_request_fn);
#else
    // 分配并初始化请求队列
    ram_disk.queue = blk_init_queue(ram_disk_request_fn, &ram_disk.spinlock);
#endif
    if (ram_disk.queue == NULL)
    {
        ret = -EINVAL;
        goto blk_queue_failed;
    }

    // 添加(注册)disk
    ram_disk.gendisk->major = ram_disk.major;            // 主设备号
    ram_disk.gendisk->first_minor = 0;                   // 起始次设备号
    ram_disk.gendisk->fops = &ram_disk_fops;             // 操作函数
    ram_disk.gendisk->private_data = &ram_disk;          // 私有数据
    ram_disk.gendisk->queue = ram_disk.queue;            // IO请求队列
    sprintf(ram_disk.gendisk->disk_name, RAM_DISK_NAME); // 名字
    set_capacity(ram_disk.gendisk, RAM_DISK_SIZE / 512); // 设备容量（单位：扇区）
    add_disk(ram_disk.gendisk);
    return 0;
blk_queue_failed:
    put_disk(ram_disk.gendisk);
gendisk_alloc_failed:
    unregister_blkdev(ram_disk.major, RAM_DISK_NAME);
register_blkdev_failed:
    kfree(ram_disk.ram_disk_buf);
alloc_buf_failed:
    return ret;
}

static void __exit RAM_DISK_exit(void)
{
    // 清除和释放IO请求队列
    blk_cleanup_queue(ram_disk.queue);
    // 释放gendisk
    del_gendisk(ram_disk.gendisk);
    put_disk(ram_disk.gendisk);
    // 注销块设备
    unregister_blkdev(ram_disk.major, RAM_DISK_NAME);
    // 释放内存
    kfree(ram_disk.ram_disk_buf);
    printk("RAM_DISK_exit\n");
}

module_init(RAM_DISK_init);
module_exit(RAM_DISK_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("liu");