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
#include <linux/platform_device.h>
#include <linux/miscdevice.h>
#define MISC_BEEP_DEVICE_NAME "beep"
#define MISC_BEEP_DEVICE_COUNT 1
#define MISC_BEEP_MINOR 144
#define MISC_BEEP_ON 0
#define MISC_BEEP_OFF 1
// MISC_BEEP设备结构体
struct MISC_BEEP
{
    struct device_node *pdevice_node;
    int gpio_num;
};

static struct MISC_BEEP misc_beep;
int MISC_BEEP_open(struct inode *inode, struct file *f)
{
    f->private_data = &misc_beep; // 设置私有数据
    return 0;
}
int MISC_BEEP_close(struct inode *inode, struct file *f)
{
    return 0;
}
ssize_t MISC_BEEP_write(struct file *f, const char __user *buf, size_t count, loff_t *pos)
{
    int ret;
    unsigned char misc_beep_Cmd = 0;
    ret = copy_from_user(&misc_beep_Cmd, buf, sizeof(misc_beep_Cmd));
    if (ret < 0)
    {
        printk("MISC_BEEP_write: copy_from_user failed!\n");
        return -EFAULT;
    }
    printk("data copy_from_user = %d\n", misc_beep_Cmd);
    if (misc_beep_Cmd == MISC_BEEP_OFF)
    {
        gpio_set_value(((struct MISC_BEEP *)(f->private_data))->gpio_num, 1);
    }
    else if (misc_beep_Cmd == MISC_BEEP_ON)
    {
        gpio_set_value(((struct MISC_BEEP *)(f->private_data))->gpio_num, 0);
    }
    return 0;
}
static const struct file_operations MISC_BEEP_op = {
    .owner = THIS_MODULE,
    .write = MISC_BEEP_write,
    .open = MISC_BEEP_open,
    .release = MISC_BEEP_close,
};

static struct miscdevice misc_beep_driver = {
    .minor = MISC_BEEP_MINOR,
    .name = MISC_BEEP_DEVICE_NAME,
    .fops = &MISC_BEEP_op,
};
int misc_beep_platform_driver_probe(struct platform_device *dev)
{
    int ret;
    misc_beep.pdevice_node = dev->dev.of_node;
    misc_beep.gpio_num = of_get_named_gpio(misc_beep.pdevice_node, "beep-gpios", 0);
    if (misc_beep.gpio_num < 0)
    {
        printk("misc_beep_driver_probe: of_get_named_gpio failed!\n");
        return -EINVAL;
    }
    ret = gpio_request(misc_beep.gpio_num, "misc_beep");
    if (ret < 0)
    {
        printk("misc_beep_driver_probe: gpio_request failed!\n");
        return -EINVAL;
    }
    ret = gpio_direction_output(misc_beep.gpio_num, 1);
    if (ret < 0)
    {
        printk("misc_beep_driver_probe: gpio_direction_output failed!\n");
        ret = -EINVAL;
        goto fail_direction;
    }

    ret = misc_register(&misc_beep_driver);
    if (ret < 0)
    {
        printk("misc_beep_driver_probe: gpio_request failed!\n");
        ret = -EINVAL;
        goto fail_register;
    }
    printk("misc_beep_platform_driver_probe\n");
    return 0;

fail_register:
fail_direction:
    gpio_free(misc_beep.gpio_num);
    return ret;
}
int misc_beep_platform_driver_remove(struct platform_device *dev)
{
    printk("misc_beep_platform_driver_remove\n");
    gpio_set_value(misc_beep.gpio_num, 1);
    gpio_free(misc_beep.gpio_num);
    return misc_deregister(&misc_beep_driver);
}
struct of_device_id of_match_table[] =
    {
        {
            .compatible = "liu,beep",
        },
        {/* Sentinel */},
};

static struct platform_driver misc_beep_platform_driver =
    {
        .driver = {
            .name = "beep",
            .of_match_table = of_match_table,
        },
        .probe = misc_beep_platform_driver_probe,
        .remove = misc_beep_platform_driver_remove,
};
static int __init MISC_BEEP_init(void)
{
    printk("MISC_BEEP_init\n");
    return platform_driver_register(&misc_beep_platform_driver);
}
static void __exit MISC_BEEP_exit(void)
{
    platform_driver_unregister(&misc_beep_platform_driver);
    printk("MISC_BEEP_exit\n");
}
module_init(MISC_BEEP_init);
module_exit(MISC_BEEP_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("liu");