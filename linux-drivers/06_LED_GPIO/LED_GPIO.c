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
#include <linux/semaphore.h>
#define LED_GPIO_NAME "LED_GPIO"
#define LED_GPIO_COUNT 1
#define LED_ON 0
#define LED_OFF 1
// LED_GPIO设备结构体
struct LED_GPIO
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
static struct LED_GPIO LED_GPIO;
int LED_GPIO_open(struct inode *inode, struct file *f)
{
    f->private_data = &LED_GPIO; // 设置私有数据
    return 0;
}
int LED_GPIO_close(struct inode *inode, struct file *f)
{
    return 0;
}
ssize_t LED_GPIO_write(struct file *f, const char __user *buf, size_t count, loff_t *pos)
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
        gpio_set_value(((struct LED_GPIO *)(f->private_data))->gpio_num, 1);
    }
    else if (LED_Cmd == LED_ON)
    {
        gpio_set_value(((struct LED_GPIO *)(f->private_data))->gpio_num, 0);
    }
    return 0;
}
static const struct file_operations LED_GPIO_op = {
    .owner = THIS_MODULE,
    .write = LED_GPIO_write,
    .open = LED_GPIO_open,
    .release = LED_GPIO_close,
};

static int __init LED_GPIO_init(void)
{
    int ret;
    // 获取设备节点
    LED_GPIO.pdevnode = of_find_node_by_path("/gpio_led");
    if (LED_GPIO.pdevnode == NULL)
    {
        printk("LED_GPIO_init: of_find_node_by_path failed!\n");
        ret = -EINVAL;
        goto failed;
    }
    // 通过设备树中该设备节点的gpios属性获取gpio编号
    LED_GPIO.gpio_num = of_get_named_gpio(LED_GPIO.pdevnode, "led-gpios", 0);
    if (LED_GPIO.gpio_num < 0)
    {
        printk("LED_GPIO_init: of_get_named_gpio failed!\n");
        ret = -EINVAL;
        goto failed;
    }
    // 申请gpio
    ret = gpio_request(LED_GPIO.gpio_num, "beep-gpio");
    if (ret < 0)
    {
        printk("LED_GPIO_init: gpio_request failed!\n");
        ret = -EINVAL;
        goto gpio_failed;
    }
    printk("gpio_num=%d\n", LED_GPIO.gpio_num);
    // 将GPIO引脚设置为输出模式
    ret = gpio_direction_output(LED_GPIO.gpio_num, 1);
    if (ret < 0)
    {
        printk("LED_GPIO_init: gpio_direction_output failed!\n");
        ret = -EINVAL;
        goto gpio_failed;
    }

    // 向linux系统申请设备编号
    if (LED_GPIO.major)
    {
        LED_GPIO.devid = MKDEV(LED_GPIO.major, LED_GPIO.minor);
        ret = register_chrdev_region(LED_GPIO.devid, LED_GPIO_COUNT, LED_GPIO_NAME);
        if (ret < 0)
        {
            printk("LED_GPIO_init: register_chrdev_region failed!\n");
            goto register_chrdev_failed;
        }
    }
    else
    {
        ret = alloc_chrdev_region(&LED_GPIO.devid, 0, LED_GPIO_COUNT, LED_GPIO_NAME);
        if (ret < 0)
        {
            printk("LED_GPIO_init: alloc_chrdev_region failed!\n");
            goto register_chrdev_failed;
        }
        LED_GPIO.major = MAJOR(LED_GPIO.devid);
        LED_GPIO.minor = MINOR(LED_GPIO.devid);
    }

    printk("LED_GPIO_init\nLED_GPIO_devid=%d LED_GPIO_major=%d\
    LED_GPIO_minor=%d\n",
           LED_GPIO.devid, LED_GPIO.major, LED_GPIO.minor);
    // 关联字符设备结构体cdev与文件操作结构体file_operations
    cdev_init(&LED_GPIO.cdev, &LED_GPIO_op);
    // 添加字符设备至cdev_map散列表中以便系统管理
    ret = cdev_add(&LED_GPIO.cdev, LED_GPIO.devid, LED_GPIO_COUNT);
    if (ret < 0)
    {
        printk("LED_GPIO_init: cdev_add failed!\n");
        goto cdev_add_failed;
    }
    // 在VFS中创建设备节点  /sys/class/${CLASS_NAME}/
    //  创建设备类
    LED_GPIO.pclass = class_create(THIS_MODULE, LED_GPIO_NAME);
    if (IS_ERR(LED_GPIO.pclass))
    {
        ret = PTR_ERR(LED_GPIO.pclass);
        goto class_create_failed;
    }
    // 在设备类下创建设备结点 /sys/class/${CLASS_NAME}/${DEVICE_NAME}/和/dev/${DEVICE_NAME}
    LED_GPIO.pdevice = device_create(LED_GPIO.pclass, NULL, LED_GPIO.devid, NULL, LED_GPIO_NAME);
    if (IS_ERR(LED_GPIO.pdevice))
    {
        ret = PTR_ERR(LED_GPIO.pdevice);
        goto device_create_failed;
    }

    return 0;

device_create_failed:
    // 销毁设备类
    class_destroy(LED_GPIO.pclass);
class_create_failed:
    // 删除字符设备
    cdev_del(&LED_GPIO.cdev);
cdev_add_failed:
    // 注销设备号
    unregister_chrdev_region(LED_GPIO.devid, LED_GPIO_COUNT);
register_chrdev_failed:
    printk("register_chrdev_failed\n");
gpio_failed:
    gpio_free(LED_GPIO.gpio_num);
failed:
    return ret;
}
static void __exit LED_GPIO_exit(void)
{

    gpio_set_value(LED_GPIO.gpio_num, 1);
    // 销毁设备
    device_destroy(LED_GPIO.pclass, LED_GPIO.devid);
    // 销毁类
    class_destroy(LED_GPIO.pclass);
    // 删除字符设备
    cdev_del(&LED_GPIO.cdev);
    // 注销设备号
    unregister_chrdev_region(LED_GPIO.devid, LED_GPIO_COUNT);
    // 释放gpio号
    gpio_free(LED_GPIO.gpio_num);

    printk("LED_GPIOLED_exit\n");
}
module_init(LED_GPIO_init);
module_exit(LED_GPIO_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("liu");