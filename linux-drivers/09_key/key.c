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
#include <asm/atomic.h>
#define KEY_NAME "KEY"
#define KEY_COUNT 1
#define KEY_ON 0
#define KEY_OFF 1
#define KEY0_VALUE 0xff         // 按键值
#define KEY0_INVALID_VALUE 0x00 // 无效的按键值
// KEY设备结构体
struct KEY
{
    dev_t devid; // 设备号
    uint major;  // 主设备号
    uint minor;  // 次设备号
    struct cdev cdev;
    struct class *pclass;
    struct device *pdevice;
    struct device_node *pdevnode;
    int gpio_num;
    atomic_t key_value;
};
static struct KEY key0;
int KEY_open(struct inode *inode, struct file *f)
{
    f->private_data = &key0; // 设置私有数据
    return 0;
}
int KEY_close(struct inode *inode, struct file *f)
{
    return 0;
}
static ssize_t KEY_read(struct file *f, char __user *buf, size_t count, loff_t *pos)
{
    int ret;
    unsigned char value;
    if (gpio_get_value(((struct KEY *)(f->private_data))->gpio_num) == 0) // 按键按下
    {
        while (!gpio_get_value(((struct KEY *)(f->private_data))->gpio_num)) // 按键松开
            ;
        atomic_set(&((struct KEY *)(f->private_data))->key_value, KEY0_VALUE);
    }
    else
    {
        atomic_set(&((struct KEY *)(f->private_data))->key_value, KEY0_INVALID_VALUE);
    }
    value = atomic_read(&((struct KEY *)(f->private_data))->key_value);
    ret = copy_to_user(buf, &value, sizeof(value));
    return ret;
}
ssize_t KEY_write(struct file *f, const char __user *buf, size_t count, loff_t *pos)
{
    return 0;
}
static const struct file_operations KEY_op = {
    .owner = THIS_MODULE,
    .open = KEY_open,
    .release = KEY_close,
    .read = KEY_read,
    .write = KEY_write,
};

static int __init KEY_init(void)
{
    int ret;
    // 获取设备节点
    key0.pdevnode = of_find_node_by_path("/key0");
    if (key0.pdevnode == NULL)
    {
        printk("KEY_init: of_find_node_by_path failed!\n");
        ret = -EINVAL;
        goto failed;
    }
    // 通过设备树中该设备节点的gpios属性获取gpio编号
    key0.gpio_num = of_get_named_gpio(key0.pdevnode, "key0-gpios", 0);
    if (key0.gpio_num < 0)
    {
        printk("KEY_init: of_get_named_gpio failed!\n");
        ret = -EINVAL;
        goto failed;
    }
    // 申请gpio
    ret = gpio_request(key0.gpio_num, "key0-gpio");
    if (ret < 0)
    {
        printk("KEY_init: gpio_request failed!\n");
        ret = -EINVAL;
        goto gpio_failed;
    }
    printk("gpio_num=%d\n", key0.gpio_num);
    // 将GPIO引脚设置为输出模式
    ret = gpio_direction_input(key0.gpio_num);
    if (ret < 0)
    {
        printk("KEY_init: gpio_direction_output failed!\n");
        ret = -EINVAL;
        goto gpio_failed;
    }

    // 向linux系统申请设备编号
    if (key0.major)
    {
        key0.devid = MKDEV(key0.major, key0.minor);
        ret = register_chrdev_region(key0.devid, KEY_COUNT, KEY_NAME);
        if (ret < 0)
        {
            printk("KEY_init: register_chrdev_region failed!\n");
            goto register_chrdev_failed;
        }
    }
    else
    {
        ret = alloc_chrdev_region(&key0.devid, 0, KEY_COUNT, KEY_NAME);
        if (ret < 0)
        {
            printk("KEY_init: alloc_chrdev_region failed!\n");
            goto register_chrdev_failed;
        }
        key0.major = MAJOR(key0.devid);
        key0.minor = MINOR(key0.devid);
    }

    printk("KEY_init\nKEY_devid=%d KEY_major=%d\
    KEY_minor=%d\n",
           key0.devid, key0.major, key0.minor);
    // 关联字符设备结构体cdev与文件操作结构体file_operations
    cdev_init(&key0.cdev, &KEY_op);
    // 添加字符设备至cdev_map散列表中以便系统管理
    ret = cdev_add(&key0.cdev, key0.devid, KEY_COUNT);
    if (ret < 0)
    {
        printk("KEY_init: cdev_add failed!\n");
        goto cdev_add_failed;
    }
    // 在VFS中创建设备节点  /sys/class/${CLASS_NAME}/
    //  创建设备类
    key0.pclass = class_create(THIS_MODULE, KEY_NAME);
    if (IS_ERR(key0.pclass))
    {
        ret = PTR_ERR(key0.pclass);
        goto class_create_failed;
    }
    // 在设备类下创建设备结点 /sys/class/${CLASS_NAME}/${DEVICE_NAME}/和/dev/${DEVICE_NAME}
    key0.pdevice = device_create(key0.pclass, NULL, key0.devid, NULL, KEY_NAME);
    if (IS_ERR(key0.pdevice))
    {
        ret = PTR_ERR(key0.pdevice);
        goto device_create_failed;
    }

    return 0;

device_create_failed:
    // 销毁设备类
    class_destroy(key0.pclass);
class_create_failed:
    // 删除字符设备
    cdev_del(&key0.cdev);
cdev_add_failed:
    // 注销设备号
    unregister_chrdev_region(key0.devid, KEY_COUNT);
register_chrdev_failed:
    printk("register_chrdev_failed\n");
gpio_failed:
    gpio_free(key0.gpio_num);
failed:
    return ret;
}
static void __exit KEY_exit(void)
{
    gpio_set_value(key0.gpio_num, 1);
    // 销毁设备
    device_destroy(key0.pclass, key0.devid);
    // 销毁类
    class_destroy(key0.pclass);
    // 删除字符设备
    cdev_del(&key0.cdev);
    // 注销设备号
    unregister_chrdev_region(key0.devid, KEY_COUNT);
    // 释放gpio号
    gpio_free(key0.gpio_num);
    printk("KEY_exit\n");
}
module_init(KEY_init);
module_exit(KEY_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("liu");