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
#include <linux/timer.h>
#define TIMER_NAME "TIMER"
#define TIMER_COUNT 1
#define TIMER_ON 0
#define TIMER_OFF 1
#define CLOSE_CMD (_IO(0xef, 1))
#define OPEN_CMD (_IO(0xef, 2))
#define SETPERIOD_CMD (_IO(0xef, 3))

// TIMER设备结构体
struct TIMER
{
    dev_t devid; // 设备号
    uint major;  // 主设备号
    uint minor;  // 次设备号
    struct cdev cdev;
    struct class *pclass;
    struct device *pdevice;
    struct device_node *pdevnode;
    int gpio_num;
    struct timer_list timer;
    unsigned long period; // 定时时间ms
    spinlock_t spinlpck;
};
static struct TIMER timer;
int TIMER_open(struct inode *inode, struct file *f)
{
    f->private_data = &timer; // 设置私有数据
    timer.period = 1000;      // 默认周期1s

    return 0;
}
static void timer_func(unsigned long parg)
{
    struct TIMER *ptimer = (struct TIMER *)parg;
    static u8 status = 1;
    unsigned long flags;
    status = !status;

    gpio_set_value(ptimer->gpio_num, status);
    spin_lock_irqsave(&ptimer->spinlpck, flags); // flags保存本地中断状态，加解锁期间不能改变
    mod_timer(&ptimer->timer, jiffies + msecs_to_jiffies(ptimer->period));
    spin_unlock_irqrestore(&ptimer->spinlpck, flags);
}
int TIMER_close(struct inode *inode, struct file *f)
{
    return 0;
}
ssize_t TIMER_write(struct file *f, const char __user *buf, size_t count, loff_t *pos)
{
    int ret;
    unsigned char timer_Cmd = 0;
    ret = copy_from_user(&timer_Cmd, buf, sizeof(timer_Cmd));
    if (ret < 0)
    {
        printk("TIMER_write: copy_from_user failed!\n");
        return -EFAULT;
    }
    printk("data copy_from_user = %d\n", timer_Cmd);
    if (timer_Cmd == TIMER_OFF)
    {
        gpio_set_value(((struct TIMER *)(f->private_data))->gpio_num, 1);
    }
    else if (timer_Cmd == TIMER_ON)
    {
        gpio_set_value(((struct TIMER *)(f->private_data))->gpio_num, 0);
    }
    return 0;
}

static long timer_unlocked_ioctl(struct file *f, unsigned int cmd, unsigned long arg)
{
    struct TIMER *ptimer = (struct TIMER *)f->private_data;
    unsigned int timer_period;
    unsigned long flags;
    printk("%d %d %d\n cmd=%u arg=%lu\n", CLOSE_CMD, OPEN_CMD, SETPERIOD_CMD, cmd, arg);
    switch (cmd)
    {
    case CLOSE_CMD:
        gpio_set_value(timer.gpio_num, 1);
        del_timer_sync(&ptimer->timer);
        break;
    case OPEN_CMD:
        spin_lock_irqsave(&ptimer->spinlpck, flags);
        timer_period = ptimer->period;
        spin_unlock_irqrestore(&ptimer->spinlpck, flags);
        // add_timer(&ptimer->timer);
        mod_timer(&ptimer->timer, jiffies + msecs_to_jiffies(timer_period)); // 也会进行初始化
        break;
    case SETPERIOD_CMD:
        spin_lock_irqsave(&ptimer->spinlpck, flags);
        ptimer->period = arg;
        spin_unlock_irqrestore(&ptimer->spinlpck, flags);
        mod_timer(&ptimer->timer, jiffies + msecs_to_jiffies(arg)); // 也会进行初始化
        break;
    }
    return 0;
}
static const struct file_operations TIMER_op = {
    .owner = THIS_MODULE,
    .write = TIMER_write,
    .open = TIMER_open,
    .release = TIMER_close,
    .unlocked_ioctl = timer_unlocked_ioctl,
};

static int __init TIMER_init(void)
{
    int ret;
    // 获取设备节点
    timer.pdevnode = of_find_node_by_path("/gpio_led"); // 借用设备树上gpio_led的数据
    if (timer.pdevnode == NULL)
    {
        printk("TIMER_init: of_find_node_by_path failed!\n");
        ret = -EINVAL;
        goto failed;
    }
    // 通过设备树中该设备节点的gpios属性获取gpio编号
    timer.gpio_num = of_get_named_gpio(timer.pdevnode, "led-gpios", 0);
    if (timer.gpio_num < 0)
    {
        printk("TIMER_init: of_get_named_gpio failed!\n");
        ret = -EINVAL;
        goto failed;
    }
    // 申请gpio
    ret = gpio_request(timer.gpio_num, "led-gpio");
    if (ret < 0)
    {
        printk("TIMER_init: gpio_request failed!\n");
        ret = -EINVAL;
        goto gpio_failed;
    }
    printk("gpio_num=%d\n", timer.gpio_num);
    // 将GPIO引脚设置为输出模式
    ret = gpio_direction_output(timer.gpio_num, 1);
    if (ret < 0)
    {
        printk("TIMER_init: gpio_direction_output failed!\n");
        ret = -EINVAL;
        goto gpio_failed;
    }

    // 向linux系统申请设备编号
    if (timer.major)
    {
        timer.devid = MKDEV(timer.major, timer.minor);
        ret = register_chrdev_region(timer.devid, TIMER_COUNT, TIMER_NAME);
        if (ret < 0)
        {
            printk("TIMER_init: register_chrdev_region failed!\n");
            goto register_chrdev_failed;
        }
    }
    else
    {
        ret = alloc_chrdev_region(&timer.devid, 0, TIMER_COUNT, TIMER_NAME);
        if (ret < 0)
        {
            printk("TIMER_init: alloc_chrdev_region failed!\n");
            goto register_chrdev_failed;
        }
        timer.major = MAJOR(timer.devid);
        timer.minor = MINOR(timer.devid);
    }

    printk("TIMER_init\nTIMER_devid=%d TIMER_major=%d\
    TIMER_minor=%d\n",
           timer.devid, timer.major, timer.minor);
    // 关联字符设备结构体cdev与文件操作结构体file_operations
    cdev_init(&timer.cdev, &TIMER_op);
    // 添加字符设备至cdev_map散列表中以便系统管理
    ret = cdev_add(&timer.cdev, timer.devid, TIMER_COUNT);
    if (ret < 0)
    {
        printk("TIMER_init: cdev_add failed!\n");
        goto cdev_add_failed;
    }
    // 在VFS中创建设备节点  /sys/class/${CLASS_NAME}/
    //  创建设备类
    timer.pclass = class_create(THIS_MODULE, TIMER_NAME);
    if (IS_ERR(timer.pclass))
    {
        ret = PTR_ERR(timer.pclass);
        goto class_create_failed;
    }
    // 在设备类下创建设备结点 /sys/class/${CLASS_NAME}/${DEVICE_NAME}/和/dev/${DEVICE_NAME}
    timer.pdevice = device_create(timer.pclass, NULL, timer.devid, NULL, TIMER_NAME);
    if (IS_ERR(timer.pdevice))
    {
        ret = PTR_ERR(timer.pdevice);
        goto device_create_failed;
    }
    init_timer(&timer.timer);
    timer.timer.function = timer_func;
    timer.timer.data = (unsigned long)&timer;
    timer.timer.expires = jiffies + msecs_to_jiffies(500);
    // add_timer(&timer.timer);
    return 0;

device_create_failed:
    // 销毁设备类
    class_destroy(timer.pclass);
class_create_failed:
    // 删除字符设备
    cdev_del(&timer.cdev);
cdev_add_failed:
    // 注销设备号
    unregister_chrdev_region(timer.devid, TIMER_COUNT);
register_chrdev_failed:
    printk("register_chrdev_failed\n");
gpio_failed:
    gpio_free(timer.gpio_num);
failed:
    return ret;
}
static void __exit TIMER_exit(void)
{
    gpio_set_value(timer.gpio_num, 1);
    // 删除定时器
    del_timer(&timer.timer);
    // 销毁设备
    device_destroy(timer.pclass, timer.devid);
    // 销毁类
    class_destroy(timer.pclass);
    // 删除字符设备
    cdev_del(&timer.cdev);
    // 注销设备号
    unregister_chrdev_region(timer.devid, TIMER_COUNT);
    // 释放gpio号
    gpio_free(timer.gpio_num);
    printk("TIMER_exit\n");
}
module_init(TIMER_init);
module_exit(TIMER_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("liu");