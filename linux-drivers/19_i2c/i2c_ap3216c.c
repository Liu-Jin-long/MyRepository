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
#include "ap3216c.h"
#define AP3216C_NAME "ap3216c"
#define AP3216C_COUNT 1

struct AP3216C
{
    dev_t devid; // 设备号
    uint major;  // 主设备号
    uint minor;  // 次设备号
    struct cdev cdev;
    struct class *pclass;
    struct device *pdevice;
    void *private_data;
    unsigned short ir, als, ps; // 环境光强度(ALS)、接近距离(PS)和红外线强度(IR)
};
static struct AP3216C ap3216c;

// 从ap3216c读出多个寄存器的数据
static int ap3216c_read_regs(struct AP3216C *ap3216c, u8 reg, void *value, u8 length)
{
    int ret;
    struct i2c_client *client = (struct i2c_client *)ap3216c->private_data;
    struct i2c_msg msg[2];
    // 偏移读写位置
    msg[0].addr = client->addr; // 从机地址
    msg[0].flags = 0;           // 写
    msg[0].buf = &reg;          // 要写入的地址即寄存器地址
    msg[0].len = 1;             // 写入长度
    //  连续读数据
    msg[1].addr = client->addr; // 从机地址
    msg[1].flags = I2C_M_RD;    // 读
    msg[1].buf = value;         // 读出的数据
    msg[1].len = length;        // 读出长度

    ret = i2c_transfer(client->adapter, msg, 2);
    if (ret == 2)
    {
        ret = 0;
    }
    else
    {
        printk("i2c rd failed=%d reg=%06x len=%d\n", ret, reg, length);
        ret = -EREMOTEIO;
    }
    return ret;
}
// 从ap3216c读出1个寄存器的数据
static unsigned char ap3216c_read_reg(struct AP3216C *ap3216c, u8 reg)
{
    u8 value;
    ap3216c_read_regs(ap3216c, reg, &value, 1);
    return value;
#if 0
    return i2c_smbus_read_byte_data((struct i2c_client *)ap3216c->private_data, reg);
#endif
}
// 向ap3216c写入多个寄存器的数据
static int ap3216c_write_regs(struct AP3216C *ap3216c, u8 reg, const void *buf, u8 length)
{
    int ret;
    u8 send_buf[256];
    struct i2c_client *client = (struct i2c_client *)ap3216c->private_data;
    // 构建发送数据 寄存器首地址和要写入的数据
    struct i2c_msg msg;
    send_buf[0] = reg; // 要写入的地址即寄存器地址
    memcpy(&send_buf[1], buf, length);
    // 偏移读写位置+连续写数据
    msg.addr = client->addr; // 从机地址
    msg.flags = 0;           // 写
    msg.buf = send_buf;      // 要写入的地址和数据
    msg.len = length + 1;    // 写入长度
    ret = i2c_transfer(client->adapter, &msg, 1);
    return ret;
}
// 向ap3216c写入1个寄存器的数据
static int ap3216c_write_reg(struct AP3216C *ap3216c, u8 reg, u8 data)
{
    u8 buf = data;
    return ap3216c_write_regs(ap3216c, reg, &buf, 1);
#if 0
    return i2c_smbus_write_byte_data((struct i2c_client *)ap3216c->private_data, reg, data);
#endif
}

static void ap3216c_read_data(struct AP3216C *ap3216c)
{
    u8 i;
    unsigned char buf[6];
    // 循环读取所有传感器数据
    for (i = 0; i < 6; i++)
    {
        buf[i] = ap3216c_read_reg(ap3216c, AP3216C_IRDATALOW + i);
    }

    if (buf[0] & 0x80) // IR_OF位为1时 IR&PS数据无效
    {
        ap3216c->ir = 0;
        ap3216c->ps = 0;
    }
    else
    {
        ap3216c->ir = ((unsigned short)buf[1] << 2) | (buf[0] & 0X03);
        ap3216c->ps = ((unsigned short)(buf[5] & 0X3F) << 4) | (buf[4] & 0X0F);
    }
    ap3216c->als = ((unsigned short)buf[3] << 8) | buf[2];
    printk("ap3216c_read_data lr = %d, als = %d, ps = %d\n", ap3216c->ir, ap3216c->als, ap3216c->ps);
}
int AP3216C_open(struct inode *inode, struct file *f)
{
    f->private_data = &ap3216c;
    // 初始化ap3216c
    ap3216c_write_reg(&ap3216c, AP3216C_SYSTEMCONG, 0x04);
    mdelay(50); // ap3216c复位最少10ms
    ap3216c_write_reg(&ap3216c, AP3216C_SYSTEMCONG, 0x03);

    printk("AP3216C_open\n");
    return 0;
}
int AP3216C_close(struct inode *inode, struct file *f)
{
    f->private_data = &ap3216c;

    printk("AP3216C_close\n");
    return 0;
}
ssize_t AP3216C_read(struct file *f, char __user *buf, size_t count, loff_t *pos)
{
    int ret;
    short data[3];
    struct AP3216C *ap3216c = (struct AP3216C *)f->private_data;
    ap3216c_read_data(ap3216c);
    data[0] = ap3216c->ir;
    data[1] = ap3216c->als;
    data[2] = ap3216c->ps;
    ret = copy_to_user(buf, data, sizeof(data));
    printk("AP3216C_read\n");
    return ret;
}
static const struct file_operations AP3216C_op = {
    .owner = THIS_MODULE,
    .read = AP3216C_read,
    .open = AP3216C_open,
    .release = AP3216C_close,
};
int ap3216c_driver_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
    int ret;
    printk("ap3216c_driver_probe\n");

    // 搭建字符设备驱动框架
    // 向linux系统申请设备编号
    if (ap3216c.major)
    {
        ap3216c.devid = MKDEV(ap3216c.major, ap3216c.minor);
        ret = register_chrdev_region(ap3216c.devid, AP3216C_COUNT, AP3216C_NAME);
        if (ret < 0)
        {
            printk("AP3216C_init: register_chrdev_region failed!\n");
            goto register_chrdev_failed;
        }
    }
    else
    {
        ret = alloc_chrdev_region(&ap3216c.devid, 0, AP3216C_COUNT, AP3216C_NAME);
        if (ret < 0)
        {
            printk("AP3216C_init: alloc_chrdev_region failed!\n");
            goto register_chrdev_failed;
        }
        ap3216c.major = MAJOR(ap3216c.devid);
        ap3216c.minor = MINOR(ap3216c.devid);
    }
    printk("ap3216c.major=%d  ap3216c.minor=%d\n", ap3216c.major, ap3216c.minor);
    // 关联字符设备结构体cdev与文件操作结构体file_operations
    cdev_init(&ap3216c.cdev, &AP3216C_op);
    // 添加字符设备至cdev_map散列表中以便系统管理
    ret = cdev_add(&ap3216c.cdev, ap3216c.devid, AP3216C_COUNT);
    if (ret < 0)
    {
        printk("AP3216C_init: cdev_add failed!\n");
        goto cdev_add_failed;
    }
    // 在VFS中创建设备节点  /sys/class/${CLASS_NAME}/
    //  创建设备类
    ap3216c.pclass = class_create(THIS_MODULE, AP3216C_NAME);
    if (IS_ERR(ap3216c.pclass))
    {
        ret = PTR_ERR(ap3216c.pclass);
        goto class_create_failed;
    }
    // 在VFS中创建设备节点  /sys/class/${CLASS_NAME}/
    ap3216c.pdevice = device_create(ap3216c.pclass, NULL, ap3216c.devid, NULL, AP3216C_NAME);
    if (IS_ERR(ap3216c.pdevice))
    {
        ret = PTR_ERR(ap3216c.pdevice);
        goto device_create_failed;
    }

    ap3216c.private_data = client;
    return 0;

device_create_failed:
    // 销毁设备类
    class_destroy(ap3216c.pclass);
class_create_failed:
    // 删除字符设备
    cdev_del(&ap3216c.cdev);
cdev_add_failed:
    // 注销设备号
    unregister_chrdev_region(ap3216c.devid, AP3216C_COUNT);
register_chrdev_failed:
    printk("register_chrdev_failed\n");
    return ret;
}
int ap3216c_driver_remove(struct i2c_client *client)
{
    // 销毁设备
    device_destroy(ap3216c.pclass, ap3216c.devid);
    // 销毁类
    class_destroy(ap3216c.pclass);
    // 删除字符设备
    cdev_del(&ap3216c.cdev);
    // 注销设备号
    unregister_chrdev_region(ap3216c.devid, AP3216C_COUNT);
    printk("ap3216c_driver_remove\n");
    return 0;
}
struct of_device_id ap3216c_of_match[] = {
    {.compatible = "liu,ap3216c"},
    {},
};
struct i2c_device_id ap3216c_id[] = {
    {"liu,ap3216c", 0},
    {},
};
static struct i2c_driver ap3216c_driver = {
    .driver = {
        .name = "ap3216c-liu",
        .owner = THIS_MODULE,
        .of_match_table = ap3216c_of_match,
    },
    .id_table = ap3216c_id,
    .probe = ap3216c_driver_probe,
    .remove = ap3216c_driver_remove,
};

static int __init I2C_AP3216C_init(void)
{
    int ret;
    printk("I2C_AP3216C_init\n");
    ret = i2c_add_driver(&ap3216c_driver);
    return ret;
}

static void __exit I2C_AP3216C_exit(void)
{
    i2c_del_driver(&ap3216c_driver);
    printk("I2C_AP3216C_exit\n");
}

module_init(I2C_AP3216C_init);
module_exit(I2C_AP3216C_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("liu");