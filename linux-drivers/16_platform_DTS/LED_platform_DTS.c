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
#include <linux/platform_device.h>
#include <linux/gpio.h>
#define LED_ON 1
#define LED_OFF 0
#define LED_PLATFORM_DEVICE_DTS_NAME "LED_PLATFORM_DEVICE_DTS"
#define LED_PLATFORM_DEVICE_DTS_COUNT 1

// LED_PLATFORM_DTS设备结构体
struct LED
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
static struct LED LED_PLATFORM_DTS;
int LED_PLATFORM_DTS_open(struct inode *inode, struct file *f)
{
    f->private_data = &LED_PLATFORM_DTS; // 设置私有数据
    return 0;
}
int LED_PLATFORM_DTS_close(struct inode *inode, struct file *f)
{
    return 0;
}
ssize_t LED_PLATFORM_DTS_write(struct file *f, const char __user *buf, size_t count, loff_t *pos)
{
    int ret;
    unsigned char LED_Cmd = 0;
    ret = copy_from_user(&LED_Cmd, buf, sizeof(LED_Cmd));
    if (ret < 0)
    {
        printk("LED_GPIO_write: copy_from_user failed!\n");
        return -EFAULT;
    }
    printk("data copy_from_user = %d\n", LED_Cmd);
    if (LED_Cmd == LED_OFF)
    {
        gpio_set_value(((struct LED *)(f->private_data))->gpio_num, 1);
    }
    else if (LED_Cmd == LED_ON)
    {
        gpio_set_value(((struct LED *)(f->private_data))->gpio_num, 0);
    }
    return 0;
}
static const struct file_operations LED_PLATFORM_DTS_op = {
    .owner = THIS_MODULE,
    .write = LED_PLATFORM_DTS_write,
    .open = LED_PLATFORM_DTS_open,
    .release = LED_PLATFORM_DTS_close,
};

int LED_platform_driver_probe(struct platform_device *dev)
{
    int ret;
    // 向linux系统申请设备编号
    if (LED_PLATFORM_DTS.major)
    {
        LED_PLATFORM_DTS.devid = MKDEV(LED_PLATFORM_DTS.major, LED_PLATFORM_DTS.minor);
        ret = register_chrdev_region(LED_PLATFORM_DTS.devid, LED_PLATFORM_DEVICE_DTS_COUNT, LED_PLATFORM_DEVICE_DTS_NAME);
        if (ret < 0)
        {
            printk("LED_platform_driver_probe: register_chrdev_region failed!\n");
            goto register_chrdev_failed;
        }
    }
    else
    {
        ret = alloc_chrdev_region(&LED_PLATFORM_DTS.devid, 0, LED_PLATFORM_DEVICE_DTS_COUNT, LED_PLATFORM_DEVICE_DTS_NAME);
        if (ret < 0)
        {
            printk("LED_platform_driver_probe: alloc_chrdev_region failed!\n");
            goto register_chrdev_failed;
        }
        LED_PLATFORM_DTS.major = MAJOR(LED_PLATFORM_DTS.devid);
        LED_PLATFORM_DTS.minor = MINOR(LED_PLATFORM_DTS.devid);
    }

    printk("LED_platform_driver_probe\nLED_PLATFORM_DTS_devid=%d LED_PLATFORM_DTS_major=%d\
    LED_PLATFORM_DTS_minor=%d\n",
           LED_PLATFORM_DTS.devid, LED_PLATFORM_DTS.major, LED_PLATFORM_DTS.minor);
    // 关联字符设备结构体cdev与文件操作结构体file_operations
    cdev_init(&LED_PLATFORM_DTS.cdev, &LED_PLATFORM_DTS_op);
    // 添加字符设备至cdev_map散列表中以便系统管理
    ret = cdev_add(&LED_PLATFORM_DTS.cdev, LED_PLATFORM_DTS.devid, LED_PLATFORM_DEVICE_DTS_COUNT);
    if (ret < 0)
    {
        printk("LED_platform_driver_probe: cdev_add failed!\n");
        goto cdev_add_failed;
    }
    // 在VFS中创建设备节点  /sys/class/${CLASS_NAME}/
    //  创建设备类
    LED_PLATFORM_DTS.pclass = class_create(THIS_MODULE, LED_PLATFORM_DEVICE_DTS_NAME);
    if (IS_ERR(LED_PLATFORM_DTS.pclass))
    {
        ret = PTR_ERR(LED_PLATFORM_DTS.pclass);
        goto class_create_failed;
    }
    // 在设备类下创建设备结点 /sys/class/${CLASS_NAME}/${DEVICE_NAME}/和/dev/${DEVICE_NAME}
    LED_PLATFORM_DTS.pdevice = device_create(LED_PLATFORM_DTS.pclass, NULL, LED_PLATFORM_DTS.devid, NULL, LED_PLATFORM_DEVICE_DTS_NAME);
    if (IS_ERR(LED_PLATFORM_DTS.pdevice))
    {
        ret = PTR_ERR(LED_PLATFORM_DTS.pdevice);
        goto device_create_failed;
    }
    // 获取设备节点
    // LED_PLATFORM_DTS.pdevnode = of_find_node_by_path("/gpio_led");
    // if (LED_PLATFORM_DTS.pdevnode == NULL)
    // {
    //     printk("LED_PLATFORM_DTS_init: of_find_node_by_path failed!\n");
    //     ret = -EINVAL;
    //     goto failed;
    // }

    // 直接通过platform_device获取设备节点
    LED_PLATFORM_DTS.pdevnode = dev->dev.of_node;
    // 通过设备树中该设备节点的gpios属性获取gpio编号
    LED_PLATFORM_DTS.gpio_num = of_get_named_gpio(LED_PLATFORM_DTS.pdevnode, "led-gpios", 0);
    if (LED_PLATFORM_DTS.gpio_num < 0)
    {
        printk("LED_platform_driver_probe: of_get_named_gpio failed!\n");
        ret = -EINVAL;
        goto failed;
    }
    // 申请gpio
    ret = gpio_request(LED_PLATFORM_DTS.gpio_num, "led-gpio");
    if (ret < 0)
    {
        printk("LED_platform_driver_probe: gpio_request failed!\n");
        ret = -EINVAL;
        goto gpio_failed;
    }
    printk("gpio_num=%d\n", LED_PLATFORM_DTS.gpio_num);
    // 将GPIO引脚设置为输出模式
    ret = gpio_direction_output(LED_PLATFORM_DTS.gpio_num, 1);
    if (ret < 0)
    {
        printk("LED_platform_driver_probe: gpio_direction_output failed!\n");
        ret = -EINVAL;
        goto gpio_failed;
    }
    return 0;

device_create_failed:
    // 销毁设备类
    class_destroy(LED_PLATFORM_DTS.pclass);
class_create_failed:
    // 删除字符设备
    cdev_del(&LED_PLATFORM_DTS.cdev);
cdev_add_failed:
    // 注销设备号
    unregister_chrdev_region(LED_PLATFORM_DTS.devid, LED_PLATFORM_DEVICE_DTS_COUNT);
register_chrdev_failed:
    printk("register_chrdev_failed\n");
gpio_failed:
    gpio_free(LED_PLATFORM_DTS.gpio_num);
failed:
    return ret;
}
int LED_platform_driver_remove(struct platform_device *dev)
{
    gpio_set_value(LED_PLATFORM_DTS.gpio_num, 1);
    // 销毁设备
    device_destroy(LED_PLATFORM_DTS.pclass, LED_PLATFORM_DTS.devid);
    // 销毁类
    class_destroy(LED_PLATFORM_DTS.pclass);
    // 删除字符设备
    cdev_del(&LED_PLATFORM_DTS.cdev);
    // 注销设备号
    unregister_chrdev_region(LED_PLATFORM_DTS.devid, LED_PLATFORM_DEVICE_DTS_COUNT);
    // 释放gpio号
    gpio_free(LED_PLATFORM_DTS.gpio_num);

    printk("LED_platform_driver_remove\n");
    return 0;
}
struct of_device_id LED_of_match[] =
    {
        {.compatible = "liu,gpio_led"},
        {/* Sentinel */},
};
static struct platform_driver LED_platform_driver = {
    .driver =
        {
            .name = "gpio_led", // 驱动名字，用于和设备匹配
            .of_match_table = LED_of_match,
        },
    .probe = LED_platform_driver_probe,
    .remove = LED_platform_driver_remove,
};
static int __init LED_PLATFORM_DTS_init(void)
{
    printk("LED_PLATFORM_DTS_init\n");
    return platform_driver_register(&LED_platform_driver);
}
static void __exit LED_PLATFORM_DTS_exit(void)
{
    platform_driver_unregister(&LED_platform_driver);
    printk("LED_PLATFORM_DTS_init\n");
}
module_init(LED_PLATFORM_DTS_init);
module_exit(LED_PLATFORM_DTS_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("liu");