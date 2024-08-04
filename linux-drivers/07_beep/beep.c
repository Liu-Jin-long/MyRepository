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
#include <linux/of_gpio.h>
#include <linux/gpio.h>
#define BEEP_NAME "BEEP"
#define BEEP_COUNT 1
#define BEEP_ON 0
#define BEEP_OFF 1
// BEEP设备结构体
struct BEEP
{
    dev_t devid; // 设备号
    uint major;  // 主设备号
    uint minor;  // 次设备号
    struct cdev cdev;
    struct class *pclass;
    struct device *pdevice;
    struct device_node *pdevnode;
    int gpio_num;
};
static struct BEEP beep;
int BEEP_open(struct inode *inode, struct file *f)
{
    f->private_data = &beep; // 设置私有数据
    return 0;
}
int BEEP_close(struct inode *inode, struct file *f)
{
    return 0;
}
ssize_t BEEP_write(struct file *f, const char __user *buf, size_t count, loff_t *pos)
{
    int ret;
    unsigned char beep_Cmd = 0;
    ret = copy_from_user(&beep_Cmd, buf, sizeof(beep_Cmd));
    if (ret < 0)
    {
        printk("BEEP_write: copy_from_user failed!\n");
        return -EFAULT;
    }
    printk("data copy_from_user = %d\n", beep_Cmd);
    if (beep_Cmd == BEEP_OFF)
    {
        gpio_set_value(((struct BEEP *)(f->private_data))->gpio_num, 1);
    }
    else if (beep_Cmd == BEEP_ON)
    {
        gpio_set_value(((struct BEEP *)(f->private_data))->gpio_num, 0);
    }
    return 0;
}
static const struct file_operations BEEP_op = {
    .owner = THIS_MODULE,
    .write = BEEP_write,
    .open = BEEP_open,
    .release = BEEP_close,
};

static int __init BEEP_init(void)
{
    int ret;
    // 获取设备节点
    beep.pdevnode = of_find_node_by_path("/beep");
    if (beep.pdevnode == NULL)
    {
        printk("BEEP_init: of_find_node_by_path failed!\n");
        ret = -EINVAL;
        goto failed;
    }
    // 通过设备树中该设备节点的gpios属性获取gpio编号
    beep.gpio_num = of_get_named_gpio(beep.pdevnode, "beep-gpios", 0);
    if (beep.gpio_num < 0)
    {
        printk("BEEP_init: of_get_named_gpio failed!\n");
        ret = -EINVAL;
        goto failed;
    }
    // 申请gpio
    ret = gpio_request(beep.gpio_num, "beep-gpio");
    if (ret < 0)
    {
        printk("BEEP_init: gpio_request failed!\n");
        ret = -EINVAL;
        goto gpio_failed;
    }
    printk("gpio_num=%d\n", beep.gpio_num);
    // 将GPIO引脚设置为输出模式
    ret = gpio_direction_output(beep.gpio_num, 1);
    if (ret < 0)
    {
        printk("BEEP_init: gpio_direction_output failed!\n");
        ret = -EINVAL;
        goto gpio_failed;
    }

    // 向linux系统申请设备编号
    if (beep.major)
    {
        beep.devid = MKDEV(beep.major, beep.minor);
        ret = register_chrdev_region(beep.devid, BEEP_COUNT, BEEP_NAME);
        if (ret < 0)
        {
            printk("BEEP_init: register_chrdev_region failed!\n");
            goto register_chrdev_failed;
        }
    }
    else
    {
        ret = alloc_chrdev_region(&beep.devid, 0, BEEP_COUNT, BEEP_NAME);
        if (ret < 0)
        {
            printk("BEEP_init: alloc_chrdev_region failed!\n");
            goto register_chrdev_failed;
        }
        beep.major = MAJOR(beep.devid);
        beep.minor = MINOR(beep.devid);
    }

    printk("BEEP_init\nBEEP_devid=%d BEEP_major=%d\
    BEEP_minor=%d\n",
           beep.devid, beep.major, beep.minor);
    // 关联字符设备结构体cdev与文件操作结构体file_operations
    cdev_init(&beep.cdev, &BEEP_op);
    // 添加字符设备至cdev_map散列表中以便系统管理
    ret = cdev_add(&beep.cdev, beep.devid, BEEP_COUNT);
    if (ret < 0)
    {
        printk("BEEP_init: cdev_add failed!\n");
        goto cdev_add_failed;
    }
    // 在VFS中创建设备节点  /sys/class/${CLASS_NAME}/
    //  创建设备类
    beep.pclass = class_create(THIS_MODULE, BEEP_NAME);
    if (IS_ERR(beep.pclass))
    {
        ret = PTR_ERR(beep.pclass);
        goto class_create_failed;
    }
     // 在设备类下创建设备结点 /sys/class/${CLASS_NAME}/${DEVICE_NAME}/和/dev/${DEVICE_NAME}
    beep.pdevice = device_create(beep.pclass, NULL, beep.devid, NULL, BEEP_NAME);
    if (IS_ERR(beep.pdevice))
    {
        ret = PTR_ERR(beep.pdevice);
        goto device_create_failed;
    }

    return 0;

device_create_failed:
    // 销毁设备类
    class_destroy(beep.pclass);
class_create_failed:
    // 删除字符设备
    cdev_del(&beep.cdev);
cdev_add_failed:
    // 注销设备号
    unregister_chrdev_region(beep.devid, BEEP_COUNT);
register_chrdev_failed:
    printk("register_chrdev_failed\n");
gpio_failed:
    gpio_free(beep.gpio_num);
failed:
    return ret;
}
static void __exit BEEP_exit(void)
{
    gpio_set_value(beep.gpio_num, 1);
    // 销毁设备
    device_destroy(beep.pclass, beep.devid);
    // 销毁类
    class_destroy(beep.pclass);
    // 删除字符设备
    cdev_del(&beep.cdev);
    // 注销设备号
    unregister_chrdev_region(beep.devid, BEEP_COUNT);
    // 释放gpio号
    gpio_free(beep.gpio_num);
    printk("BEEP_exit\n");
}
module_init(BEEP_init);
module_exit(BEEP_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("liu");