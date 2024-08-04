#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/ide.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/errno.h>
#include <linux/gpio.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <asm/mach/map.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <linux/platform_device.h>

#define RESOURCE_NUM 5
/* 寄存器物理地址 */
#define CCM_CCGR1_BASE (0X020C406C)
#define SW_MUX_GPIO1_IO03_BASE (0X020E0068)
#define SW_PAD_GPIO1_IO03_BASE (0X020E02F4)
#define GPIO1_DR_BASE (0X0209C000)
#define GPIO1_GDIR_BASE (0X0209C004)

#define LED_ON 1
#define LED_OFF 0

#define LED_PLATFORM_DRIVER_DEV_NAME "LED_platform_match_name"
#define LED_PLATFORM_DRIVER_DEV_COUNT 1
// 映射后的虚拟地址
static void __iomem *IMX6U_CCM_CCGR1;
static void __iomem *SW_MUX_GPIO1_IO03;
static void __iomem *SW_PAD_GPIO1_IO03;
static void __iomem *GPIO1_DR;
static void __iomem *GPIO1_GDIR;

// LED_PLATFORM_DRIVER_DEV设备结构体
struct LED_PLATFORM_DRIVER_DEV
{
    dev_t devid; // 设备号
    uint major;  // 主设备号
    uint minor;  // 次设备号
    struct cdev cdev;
    struct class *pclass;
    struct device *pdevice;
};
static struct LED_PLATFORM_DRIVER_DEV LED_platform_driver_dev;
int LED_PLATFORM_DRIVER_DEV_open(struct inode *inode, struct file *f)
{
    f->private_data = &LED_platform_driver_dev; // 设置私有数据
    return 0;
}
int LED_PLATFORM_DRIVER_DEV_close(struct inode *inode, struct file *f)
{
    return 0;
}
ssize_t LED_PLATFORM_DRIVER_DEV_read(struct file *f, char __user *buf, size_t count, loff_t *pos)
{
    return 0;
}
ssize_t LED_PLATFORM_DRIVER_DEV_write(struct file *f, const char __user *buf, size_t count, loff_t *pos)
{
    int ret;
    u32 val;
    unsigned char LED_Cmd = 0;
    ret = copy_from_user(&LED_Cmd, buf, sizeof(LED_Cmd));
    if (ret < 0)
    {
        printk("LED_PLATFORM_DRIVER_DEV_write: copy_from_user failed!\n");
        return -EFAULT;
    }
    if (LED_Cmd == LED_OFF)
    {
        val = readl(GPIO1_DR);
        val |= (1 << 3);
        writel(val, GPIO1_DR);
    }
    else if (LED_Cmd == LED_ON)
    {
        val = readl(GPIO1_DR);
        val &= ~(1 << 3);
        writel(val, GPIO1_DR);
    }
    return 0;
}
static const struct file_operations LED_PLATFORM_DRIVER_DEV_op = {
    .owner = THIS_MODULE,
    .open = LED_PLATFORM_DRIVER_DEV_open,
    .release = LED_PLATFORM_DRIVER_DEV_close,
    .read = LED_PLATFORM_DRIVER_DEV_read,
    .write = LED_PLATFORM_DRIVER_DEV_write,
};
// 无论先挂载驱动还是设备，一旦双方都挂载就会匹配到，从而执行驱动的probe函数
static int LED_platform_driver_probe(struct platform_device *dev)
{
    int i, ret;
    u32 val;
    struct resource *LED_platform_device_resources[RESOURCE_NUM];
    int resources_size[RESOURCE_NUM];
    for (i = 0; i < RESOURCE_NUM; i++)
    {
        LED_platform_device_resources[i] = platform_get_resource(dev, IORESOURCE_MEM, i);
        if (LED_platform_device_resources[i] == NULL)
        {
            printk("LED_platform_driver_probe: platform_get_resource failed!\n");
            dev_err(&dev->dev, "No MEM resource for always on\n");
            return -ENXIO;
        }
        resources_size[i] = resource_size(LED_platform_device_resources[i]);
    }

    // 寄存器地址映射
    IMX6U_CCM_CCGR1 = ioremap(LED_platform_device_resources[0]->start, resources_size[0]);
    SW_MUX_GPIO1_IO03 = ioremap(LED_platform_device_resources[1]->start, resources_size[1]);
    SW_PAD_GPIO1_IO03 = ioremap(LED_platform_device_resources[2]->start, resources_size[2]);
    GPIO1_DR = ioremap(LED_platform_device_resources[3]->start, resources_size[3]);
    GPIO1_GDIR = ioremap(LED_platform_device_resources[4]->start, resources_size[4]);

    // 使能 GPIO1 时钟
    val = readl(IMX6U_CCM_CCGR1);
    val &= ~(3 << 26); /* 清除以前的设置 */
    val |= (3 << 26);  /* 设置新值 */
    writel(val, IMX6U_CCM_CCGR1);

    // 设置 GPIO1_IO03 的复用功能，将其复用为GPIO1_IO03，最后设置IO属性
    writel(5, SW_MUX_GPIO1_IO03);
    // 寄存器 SW_PAD_GPIO1_IO03 设置 IO 属性
    /*bit 16:0 HYS关闭
     *bit [15:14]: 00 默认下拉
     *bit [13]: 0 kepper功能
     *bit [12]: 1 pull/keeper使能
     *bit [11]: 0 关闭开路输出
     *bit [7:6]: 10 速度100Mhz
     *bit [5:3]: 110 R0/6驱动能力
     *bit [0]: 0 低转换率
     */
    writel(0x10B0, SW_PAD_GPIO1_IO03);

    // 设置 GPIO1_IO03 为输出功能
    val = readl(GPIO1_GDIR);
    val &= ~(1 << 3); /* 清除以前的设置 */
    val |= (1 << 3);  /* 设置为输出 */
    writel(val, GPIO1_GDIR);

    // 默认关闭 LED
    val = readl(GPIO1_DR);
    val |= (1 << 3);
    writel(val, GPIO1_DR);

    // 向linux系统申请设备编号
    if (LED_platform_driver_dev.major)
    {
        LED_platform_driver_dev.devid = MKDEV(LED_platform_driver_dev.major, LED_platform_driver_dev.minor);
        ret = register_chrdev_region(LED_platform_driver_dev.devid, LED_PLATFORM_DRIVER_DEV_COUNT, LED_PLATFORM_DRIVER_DEV_NAME);
        if (ret < 0)
        {
            printk("LED_platform_driver_probe: register_chrdev_region failed!\n");
            goto register_chrdev_failed;
        }
    }
    else
    {
        ret = alloc_chrdev_region(&LED_platform_driver_dev.devid, 0, LED_PLATFORM_DRIVER_DEV_COUNT, LED_PLATFORM_DRIVER_DEV_NAME);
        if (ret < 0)
        {
            printk("LED_platform_driver_probe: alloc_chrdev_region failed!\n");
            goto register_chrdev_failed;
        }
        LED_platform_driver_dev.major = MAJOR(LED_platform_driver_dev.devid);
        LED_platform_driver_dev.minor = MINOR(LED_platform_driver_dev.devid);
    }

    printk("LED_platform_driver_probe\nLED_PLATFORM_DRIVER_DEV_devid=%d LED_PLATFORM_DRIVER_DEV_major=%d\
    LED_PLATFORM_DRIVER_DEV_minor=%d\n",
           LED_platform_driver_dev.devid, LED_platform_driver_dev.major, LED_platform_driver_dev.minor);
    // 关联字符设备结构体cdev与文件操作结构体file_operations
    cdev_init(&LED_platform_driver_dev.cdev, &LED_PLATFORM_DRIVER_DEV_op);
    // 添加字符设备至cdev_map散列表中以便系统管理
    ret = cdev_add(&LED_platform_driver_dev.cdev, LED_platform_driver_dev.devid, LED_PLATFORM_DRIVER_DEV_COUNT);
    if (ret < 0)
    {
        printk("LED_platform_driver_probe: cdev_add failed!\n");
        goto cdev_add_failed;
    }
    // 在VFS中创建设备节点  /sys/class/${CLASS_NAME}/
    //  创建设备类
    LED_platform_driver_dev.pclass = class_create(THIS_MODULE, LED_PLATFORM_DRIVER_DEV_NAME);
    if (IS_ERR(LED_platform_driver_dev.pclass))
    {
        ret = PTR_ERR(LED_platform_driver_dev.pclass);
        goto class_create_failed;
    }
    // 在设备类下创建设备结点 /sys/class/${CLASS_NAME}/${DEVICE_NAME}/和/dev/${DEVICE_NAME}
    LED_platform_driver_dev.pdevice = device_create(LED_platform_driver_dev.pclass, NULL, LED_platform_driver_dev.devid, NULL, LED_PLATFORM_DRIVER_DEV_NAME);
    if (IS_ERR(LED_platform_driver_dev.pdevice))
    {
        ret = PTR_ERR(LED_platform_driver_dev.pdevice);
        goto device_create_failed;
    }
    return 0;

device_create_failed:
    // 销毁设备类
    class_destroy(LED_platform_driver_dev.pclass);
class_create_failed:
    // 删除字符设备
    cdev_del(&LED_platform_driver_dev.cdev);
cdev_add_failed:
    // 注销设备号
    unregister_chrdev_region(LED_platform_driver_dev.devid, LED_PLATFORM_DRIVER_DEV_COUNT);
register_chrdev_failed:
    return ret;
}
// 未匹配到设备时，卸载驱动不会执行remove函数
// 匹配到设备时，卸载设备也会执行驱动的remove函数，再卸载驱动不会再执行该函数
// 先卸载驱动时，驱动的exit函数先于remove函数执行，再卸载设备不会再执行remove函数
int LED_platform_driver_remove(struct platform_device *dev) 
{
    u32 val;
    // 关闭 LED
    val = readl(GPIO1_DR);
    val |= (1 << 3);
    writel(val, GPIO1_DR);
    // 取消映射
    iounmap(IMX6U_CCM_CCGR1);
    iounmap(SW_MUX_GPIO1_IO03);
    iounmap(SW_PAD_GPIO1_IO03);
    iounmap(GPIO1_DR);
    iounmap(GPIO1_GDIR);

    // 销毁设备
    device_destroy(LED_platform_driver_dev.pclass, LED_platform_driver_dev.devid);
    // 销毁类
    class_destroy(LED_platform_driver_dev.pclass);
    // 删除字符设备
    cdev_del(&LED_platform_driver_dev.cdev);
    // 注销设备号
    unregister_chrdev_region(LED_platform_driver_dev.devid, LED_PLATFORM_DRIVER_DEV_COUNT);
    printk("LED_platform_driver_remove\n");
    return 0;
}
struct platform_driver LED_platform_driver =
    {
        .driver = {
            .name = LED_PLATFORM_DRIVER_DEV_NAME,
        },
        .probe = LED_platform_driver_probe,
        .remove = LED_platform_driver_remove,
};

static int __init LED_PLATFORM_DRIVER_DEV_init(void)
{
    printk("LED_PLATFORM_DRIVER_DEV_init\n");
    return platform_driver_register(&LED_platform_driver);
}
static void __exit LED_PLATFORM_DRIVER_DEV_exit(void)
{
    printk("LED_PLATFORM_DRIVER_DEV_exit\n");
    platform_driver_unregister(&LED_platform_driver);
}
module_init(LED_PLATFORM_DRIVER_DEV_init);
module_exit(LED_PLATFORM_DRIVER_DEV_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("liu");