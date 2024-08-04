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
// #define DEBUG
// #define NO_BOTTOM_HALF
#define TASKLET
// #define WORK_QUEUE
#define iMX6ULL_IRQ_DEV_NAME "iMX6ULL_IRQ_DEV"
#define iMX6ULL_IRQ_DEV_COUNT 1
#define iMX6ULL_IRQ_KEY_COUNT 1
#define KEY0_PUSH_VALUE 0xff
#define KEY0_RELEASE_VALUE 0x00
#define KEY_PUSH_STATE 0x10
#define KEY_RELEASE_STATE 0x20
struct IRQ_INFO_KEY
{
    int gpio_num;                        // gpio编号
    int irq_num;                         // 中断号
    unsigned char value;                 // 按键值
    char name[15];                       // 中断名字
    irqreturn_t (*handler)(int, void *); // 中断处理函数
#ifdef TASKLET
    struct tasklet_struct tasklet;
#endif
#ifdef WORK_QUEUE
    struct work_struct work_queue;
#endif
};
// iMX6ULL_IRQ_DEV设备结构体
struct iMX6ULL_IRQ_DEV
{
    dev_t devid;                  // 设备号
    uint major;                   // 主设备号
    uint minor;                   // 次设备号
    struct cdev cdev;             // 字符设备
    struct class *pclass;         // 类
    struct device *pdevice;       // 设备
    struct device_node *pdevnode; // 设备节点
    // int gpio_num;                 // gpio编号
    atomic_t key_value;  // 按键值
    atomic_t key_status; // 按键状态
    struct IRQ_INFO_KEY irq_keys[iMX6ULL_IRQ_KEY_COUNT];
    unsigned char current_key_num; /* 当前的按键号 */
    struct timer_list timer0;
};
static struct iMX6ULL_IRQ_DEV iMX6ULL_IRQ_dev;

// 中断处理函数
irqreturn_t key0_handler(int irq_num, void *parg)
{
    struct iMX6ULL_IRQ_DEV *piMX6ULL_IRQ_dev = (struct iMX6ULL_IRQ_DEV *)parg;
#ifdef NO_BOTTOM_HALF
    piMX6ULL_IRQ_dev->current_key_num = 0;
    mod_timer(&piMX6ULL_IRQ_dev->timer0, jiffies + msecs_to_jiffies(15));
#endif
#ifdef TASKLET
    tasklet_schedule(&piMX6ULL_IRQ_dev->irq_keys[piMX6ULL_IRQ_dev->current_key_num].tasklet);
#endif
#ifdef WORK_QUEUE
    schedule_work(&piMX6ULL_IRQ_dev->irq_keys[piMX6ULL_IRQ_dev->current_key_num].work_queue);
#endif
    return IRQ_RETVAL(IRQ_HANDLED);
}
static void timer_func(unsigned long parg)
{
    int value;
    struct iMX6ULL_IRQ_DEV *piMX6ULL_IRQ_dev = (struct iMX6ULL_IRQ_DEV *)parg;
    // printk("timer_func开始\n");
    // printk("current_key_num=%d, gpio_num=%d\n", piMX6ULL_IRQ_dev->current_key_num, piMX6ULL_IRQ_dev->irq_keys[piMX6ULL_IRQ_dev->current_key_num].gpio_num);
    value = gpio_get_value(piMX6ULL_IRQ_dev->irq_keys[piMX6ULL_IRQ_dev->current_key_num].gpio_num);
    if (value == 0) // 按下动作消抖结束-->按键按下
    {
        atomic_set(&piMX6ULL_IRQ_dev->key_value, KEY0_PUSH_VALUE);
        atomic_set(&piMX6ULL_IRQ_dev->key_status, KEY_PUSH_STATE);
        printk("key%d push!\n", piMX6ULL_IRQ_dev->current_key_num);
    }
    else // 松开动作消抖结束-->按键松开
    {
        atomic_set(&piMX6ULL_IRQ_dev->key_value, KEY0_RELEASE_VALUE);
        atomic_set(&piMX6ULL_IRQ_dev->key_status, KEY_RELEASE_STATE);
        printk("key%d release!\n", piMX6ULL_IRQ_dev->current_key_num);
    }
}
#ifdef TASKLET
static void tasklet_func(unsigned long data)
{
    struct iMX6ULL_IRQ_DEV *piMX6ULL_IRQ_dev = (struct iMX6ULL_IRQ_DEV *)data;
    piMX6ULL_IRQ_dev->current_key_num = 0;
    mod_timer(&piMX6ULL_IRQ_dev->timer0, jiffies + msecs_to_jiffies(15));
}
#endif
#ifdef WORK_QUEUE
static void work_queue_func(struct work_struct *work)
{
    void *key = container_of(work, struct IRQ_INFO_KEY, work_queue);
    struct iMX6ULL_IRQ_DEV *piMX6ULL_IRQ_dev = (struct iMX6ULL_IRQ_DEV *)container_of(key, struct iMX6ULL_IRQ_DEV, irq_keys);
    piMX6ULL_IRQ_dev->current_key_num = 0;
    mod_timer(&piMX6ULL_IRQ_dev->timer0, jiffies + msecs_to_jiffies(15));
}
#endif
int iMX6ULL_IRQ_DEV_open(struct inode *inode, struct file *f)
{
    f->private_data = &iMX6ULL_IRQ_dev; // 设置私有数据
    return 0;
}
int iMX6ULL_IRQ_DEV_close(struct inode *inode, struct file *f)
{
    return 0;
}
static ssize_t iMX6ULL_IRQ_DEV_read(struct file *f, char __user *buf, size_t count, loff_t *pos)
{
    int ret;
    unsigned char value, status;
    struct iMX6ULL_IRQ_DEV *piMX6ULL_IRQ_dev = (struct iMX6ULL_IRQ_DEV *)f->private_data;
    value = atomic_read(&piMX6ULL_IRQ_dev->key_value);
    status = atomic_read(&piMX6ULL_IRQ_dev->key_status);
    if (status == KEY_PUSH_STATE) // 有按键按下
    {
        if (value == KEY0_PUSH_VALUE)
        {
            ret = copy_to_user(buf, &value, sizeof(value));
            atomic_set(&piMX6ULL_IRQ_dev->key_status, ~KEY_PUSH_STATE); // 只是为了按一下只传一次数据 打印一次
        }
        else // 松开按键
        {
            goto no_key_push;
        }
    }
    else
    {
        goto no_key_push;
    }
    return ret;
no_key_push:
    return -EINVAL;
}
ssize_t iMX6ULL_IRQ_DEV_write(struct file *f, const char __user *buf, size_t count, loff_t *pos)
{
    return 0;
}
static const struct file_operations iMX6ULL_IRQ_DEV_op = {
    .owner = THIS_MODULE,
    .open = iMX6ULL_IRQ_DEV_open,
    .release = iMX6ULL_IRQ_DEV_close,
    .read = iMX6ULL_IRQ_DEV_read,
    .write = iMX6ULL_IRQ_DEV_write,
};

static int KEY0_init(void)
{
    int ret, i, gpio_count, irq_count;
    // 获取设备节点
    iMX6ULL_IRQ_dev.pdevnode = of_find_node_by_path("/key0");
    if (iMX6ULL_IRQ_dev.pdevnode == NULL)
    {
        printk("iMX6ULL_IRQ_DEV_init: of_find_node_by_path failed!\n");
        ret = -EINVAL;
        goto failed;
    }
    // 通过设备树中该设备节点的gpios属性获取gpio编号
    for (i = 0; i < iMX6ULL_IRQ_KEY_COUNT; i++)
    {
        iMX6ULL_IRQ_dev.irq_keys[i].gpio_num = of_get_named_gpio(iMX6ULL_IRQ_dev.pdevnode, "key0-gpios", i);
        if (iMX6ULL_IRQ_dev.irq_keys[i].gpio_num < 0)
        {
            printk("iMX6ULL_IRQ_DEV_init: of_get_named_gpio failed!\n");
            ret = -EINVAL;
            goto failed;
        }
#ifdef DEBUG
        printk("of_get_named_gpio : MX6ULL_IRQ_dev.irq_keys[%d].gpio_num=%d\n", i, iMX6ULL_IRQ_dev.irq_keys[i].gpio_num);
#endif
    }

    // 初始化key所使用的gpio，并设置成中断模式
    for (i = 0; i < iMX6ULL_IRQ_KEY_COUNT; i++)
    {
        memset(iMX6ULL_IRQ_dev.irq_keys[i].name, 0, sizeof(iMX6ULL_IRQ_dev.irq_keys[i].name));
        sprintf(iMX6ULL_IRQ_dev.irq_keys[i].name, "key%d", i);
        ret = gpio_request(iMX6ULL_IRQ_dev.irq_keys[i].gpio_num, iMX6ULL_IRQ_dev.irq_keys[i].name);
        if (ret < 0)
        {
            printk("iMX6ULL_IRQ_DEV_init: gpio_request failed!\n");
            ret = -EINVAL;
            gpio_count = i;
            goto gpio_failed;
        }
#ifdef DEBUG
        printk("gpio_num=%d\n", iMX6ULL_IRQ_dev.irq_keys[i].gpio_num);
#endif
        // 将GPIO引脚设置为输出模式
        ret = gpio_direction_input(iMX6ULL_IRQ_dev.irq_keys[i].gpio_num);
        if (ret < 0)
        {
            printk("iMX6ULL_IRQ_DEV_init: gpio_direction_output failed!\n");
            ret = -EINVAL;
            gpio_count = i + 1;
            goto gpio_failed;
        }
        iMX6ULL_IRQ_dev.irq_keys[i].irq_num = irq_of_parse_and_map(iMX6ULL_IRQ_dev.pdevnode, i);
#ifdef DEBUG
        printk("irq_of_parse_and_map执行后irq_num=%d", iMX6ULL_IRQ_dev.irq_keys[i].irq_num);
#endif
#if 0
        iMX6ULL_IRQ_dev.irq_keys[i].irq_num=gpio_to_irq(iMX6ULL_IRQ_dev.irq_keys[i].gpio_num);
#endif
        if (iMX6ULL_IRQ_dev.irq_keys[i].irq_num < 0)
        {
            printk("iMX6ULL_IRQ_DEV_init: irq_of_parse_and_map failed!\n");
            ret = -EINVAL;
            gpio_count = i + 1;
            goto gpio_failed;
        }
#ifdef DEBUG
        printk("key%d : gpio_num%d irq_num=%d\n", i, iMX6ULL_IRQ_dev.irq_keys[i].gpio_num, iMX6ULL_IRQ_dev.irq_keys[i].irq_num);
#endif
    }
    gpio_count = iMX6ULL_IRQ_KEY_COUNT;
    // 申请中断 只用到了key0
    iMX6ULL_IRQ_dev.irq_keys[0].handler = key0_handler;
    // iMX6ULL_IRQ_dev.irq_keys[0].value = KEY0_RELEASE_VALUE;
    for (i = 0; i < iMX6ULL_IRQ_KEY_COUNT; i++)
    {
        ret = request_irq(iMX6ULL_IRQ_dev.irq_keys[i].irq_num, iMX6ULL_IRQ_dev.irq_keys[i].handler, IRQF_TRIGGER_FALLING | IRQF_TRIGGER_RISING, iMX6ULL_IRQ_dev.irq_keys[i].name, &iMX6ULL_IRQ_dev);
        if (ret < 0)
        {
            printk("iMX6ULL_IRQ_DEV_init: request_irq failed!\n");
            ret = -EINVAL;
            irq_count = i;
            goto irq_failed;
        }
#ifdef TASKLET
        tasklet_init(&iMX6ULL_IRQ_dev.irq_keys[i].tasklet, tasklet_func, (unsigned long)&iMX6ULL_IRQ_dev);
#endif

#ifdef WORK_QUEUE
        INIT_WORK(&iMX6ULL_IRQ_dev.irq_keys[i].work_queue, work_queue_func);
#endif
    }
    irq_count = iMX6ULL_IRQ_KEY_COUNT;
    return 0;

irq_failed:
    for (i = 0; i < irq_count; i++)
    {
        free_irq(iMX6ULL_IRQ_dev.irq_keys[i].irq_num, &iMX6ULL_IRQ_dev);
    }
gpio_failed:
    for (i = 0; i < gpio_count; i++)
    {
        gpio_free(iMX6ULL_IRQ_dev.irq_keys[i].gpio_num);
    }
failed:
    return ret;
}
static int __init iMX6ULL_IRQ_DEV_init(void)
{
    int ret;
    // 向linux系统申请设备编号
    if (iMX6ULL_IRQ_dev.major)
    {
        iMX6ULL_IRQ_dev.devid = MKDEV(iMX6ULL_IRQ_dev.major, iMX6ULL_IRQ_dev.minor);
        ret = register_chrdev_region(iMX6ULL_IRQ_dev.devid, iMX6ULL_IRQ_DEV_COUNT, iMX6ULL_IRQ_DEV_NAME);
        if (ret < 0)
        {
            printk("iMX6ULL_IRQ_DEV_init: register_chrdev_region failed!\n");
            goto register_chrdev_failed;
        }
    }
    else
    {
        ret = alloc_chrdev_region(&iMX6ULL_IRQ_dev.devid, 0, iMX6ULL_IRQ_DEV_COUNT, iMX6ULL_IRQ_DEV_NAME);
        if (ret < 0)
        {
            printk("iMX6ULL_IRQ_DEV_init: alloc_chrdev_region failed!\n");
            goto register_chrdev_failed;
        }
        iMX6ULL_IRQ_dev.major = MAJOR(iMX6ULL_IRQ_dev.devid);
        iMX6ULL_IRQ_dev.minor = MINOR(iMX6ULL_IRQ_dev.devid);
    }
    // 关联字符设备结构体cdev与文件操作结构体file_operations
    printk("iMX6ULL_IRQ_DEV_init\niMX6ULL_IRQ_DEV_devid=%d iMX6ULL_IRQ_DEV_major=%d\
    iMX6ULL_IRQ_DEV_minor=%d\n",
           iMX6ULL_IRQ_dev.devid, iMX6ULL_IRQ_dev.major, iMX6ULL_IRQ_dev.minor);
    // 关联字符设备结构体cdev与文件操作结构体file_operations
    cdev_init(&iMX6ULL_IRQ_dev.cdev, &iMX6ULL_IRQ_DEV_op);
    // 添加字符设备至cdev_map散列表中以便系统管理
    ret = cdev_add(&iMX6ULL_IRQ_dev.cdev, iMX6ULL_IRQ_dev.devid, iMX6ULL_IRQ_DEV_COUNT);
    if (ret < 0)
    {
        printk("iMX6ULL_IRQ_DEV_init: cdev_add failed!\n");
        goto cdev_add_failed;
    }
    // 在VFS中创建设备节点  /sys/class/${CLASS_NAME}/
    //  创建设备类
    iMX6ULL_IRQ_dev.pclass = class_create(THIS_MODULE, iMX6ULL_IRQ_DEV_NAME);
    if (IS_ERR(iMX6ULL_IRQ_dev.pclass))
    {
        ret = PTR_ERR(iMX6ULL_IRQ_dev.pclass);
        goto class_create_failed;
    }
    // 在设备类下创建设备结点 /sys/class/${CLASS_NAME}/${DEVICE_NAME}/和/dev/${DEVICE_NAME}
    iMX6ULL_IRQ_dev.pdevice = device_create(iMX6ULL_IRQ_dev.pclass, NULL, iMX6ULL_IRQ_dev.devid, NULL, iMX6ULL_IRQ_DEV_NAME);
    if (IS_ERR(iMX6ULL_IRQ_dev.pdevice))
    {
        ret = PTR_ERR(iMX6ULL_IRQ_dev.pdevice);
        goto device_create_failed;
    }
    ret = KEY0_init();
    if (ret < 0)
    {
        printk("iMX6ULL_IRQ_DEV_init: key_init failed!\n");
        goto failed;
    }

    init_timer(&iMX6ULL_IRQ_dev.timer0);
    iMX6ULL_IRQ_dev.timer0.function = timer_func;
    iMX6ULL_IRQ_dev.timer0.data = (unsigned long)&iMX6ULL_IRQ_dev;
    return 0;
failed:
device_create_failed:
    // 销毁设备类
    class_destroy(iMX6ULL_IRQ_dev.pclass);
class_create_failed:
    // 删除字符设备
    cdev_del(&iMX6ULL_IRQ_dev.cdev);
cdev_add_failed:
    // 注销设备号
    unregister_chrdev_region(iMX6ULL_IRQ_dev.devid, iMX6ULL_IRQ_DEV_COUNT);
register_chrdev_failed:
    printk("register_chrdev_failed\n");
    return ret;
}
static void __exit iMX6ULL_IRQ_DEV_exit(void)
{
    int i;
    // 销毁设备
    device_destroy(iMX6ULL_IRQ_dev.pclass, iMX6ULL_IRQ_dev.devid);
    // 销毁类
    class_destroy(iMX6ULL_IRQ_dev.pclass);
    // 删除字符设备
    cdev_del(&iMX6ULL_IRQ_dev.cdev);
    // 注销设备号
    unregister_chrdev_region(iMX6ULL_IRQ_dev.devid, iMX6ULL_IRQ_DEV_COUNT);
    // 释放gpio号
    for (i = 0; i < iMX6ULL_IRQ_KEY_COUNT; i++)
    {
        free_irq(iMX6ULL_IRQ_dev.irq_keys[i].irq_num, &iMX6ULL_IRQ_dev);
        gpio_free(iMX6ULL_IRQ_dev.irq_keys[i].gpio_num);
    }
    del_timer_sync(&iMX6ULL_IRQ_dev.timer0);
    printk("iMX6ULL_IRQ_DEV_exit\n");
}
module_init(iMX6ULL_IRQ_DEV_init);
module_exit(iMX6ULL_IRQ_DEV_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("liu");