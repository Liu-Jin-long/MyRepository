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
/* 寄存器物理地址 */
#define CCM_CCGR1_BASE (0X020C406C)
#define SW_MUX_GPIO1_IO03_BASE (0X020E0068)
#define SW_PAD_GPIO1_IO03_BASE (0X020E02F4)
#define GPIO1_DR_BASE (0X0209C000)
#define GPIO1_GDIR_BASE (0X0209C004)

#define LED_ON 1
#define LED_OFF 0

#define LED_NEWCHRDEV_NAME "LED_newchrdev"
#define LED_NEWCHRDEV_COUNT 1
// 映射后的虚拟地址
static void __iomem *IMX6U_CCM_CCGR1;
static void __iomem *SW_MUX_GPIO1_IO03;
static void __iomem *SW_PAD_GPIO1_IO03;
static void __iomem *GPIO1_DR;
static void __iomem *GPIO1_GDIR;

// LED_NEWCHRDEV设备结构体
struct LED_NEWCHRDEV
{
    dev_t devid; // 设备号
    uint major;  // 主设备号
    uint minor;  // 次设备号
    struct cdev cdev;
    struct class *pclass;
    struct device *pdevice;
};
static struct LED_NEWCHRDEV LED_newchrdev;
int LED_NEWCHRDEV_open(struct inode *inode, struct file *f)
{
    f->private_data = &LED_newchrdev; // 设置私有数据
    return 0;
}
int LED_NEWCHRDEV_close(struct inode *inode, struct file *f)
{
    return 0;
}
ssize_t LED_NEWCHRDEV_read(struct file *f, char __user *buf, size_t count, loff_t *pos)
{
    return 0;
}
ssize_t LED_NEWCHRDEV_write(struct file *f, const char __user *buf, size_t count, loff_t *pos)
{
    int ret;
    u32 val;
    unsigned char LED_Cmd = 0;
    ret = copy_from_user(&LED_Cmd, buf, sizeof(LED_Cmd));
    if (ret < 0)
    {
        printk("LED_NEWCHRDEV_write: copy_from_user failed!\n");
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
static const struct file_operations LED_NEWCHRDEV_op = {
    .owner = THIS_MODULE,
    .open = LED_NEWCHRDEV_open,
    .release = LED_NEWCHRDEV_close,
    .read = LED_NEWCHRDEV_read,
    .write = LED_NEWCHRDEV_write,
};

static int __init LED_NEWCHRDEV_init(void)
{
    u32 val;
    int ret;
    // 寄存器地址映射
    IMX6U_CCM_CCGR1 = ioremap(CCM_CCGR1_BASE, 4);
    SW_MUX_GPIO1_IO03 = ioremap(SW_MUX_GPIO1_IO03_BASE, 4);
    SW_PAD_GPIO1_IO03 = ioremap(SW_PAD_GPIO1_IO03_BASE, 4);
    GPIO1_DR = ioremap(GPIO1_DR_BASE, 4);
    GPIO1_GDIR = ioremap(GPIO1_GDIR_BASE, 4);

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
    if (LED_newchrdev.major)
    {
        LED_newchrdev.devid = MKDEV(LED_newchrdev.major, LED_newchrdev.minor);
        ret = register_chrdev_region(LED_newchrdev.devid, LED_NEWCHRDEV_COUNT, LED_NEWCHRDEV_NAME);
        if (ret < 0)
        {
            printk("LED_NEWCHRDEV_init: register_chrdev_region failed!\n");
            goto register_chrdev_failed;
        }
    }
    else
    {
        ret = alloc_chrdev_region(&LED_newchrdev.devid, 0, LED_NEWCHRDEV_COUNT, LED_NEWCHRDEV_NAME);
        if (ret < 0)
        {
            printk("LED_NEWCHRDEV_init: alloc_chrdev_region failed!\n");
            goto register_chrdev_failed;
        }
        LED_newchrdev.major = MAJOR(LED_newchrdev.devid);
        LED_newchrdev.minor = MINOR(LED_newchrdev.devid);
    }

    printk("LED_NEWCHRDEV_init\nLED_NEWCHRDEV_devid=%d LED_NEWCHRDEV_major=%d\
    LED_NEWCHRDEV_minor=%d\n",
           LED_newchrdev.devid, LED_newchrdev.major, LED_newchrdev.minor);
    // 关联字符设备结构体cdev与文件操作结构体file_operations
    cdev_init(&LED_newchrdev.cdev, &LED_NEWCHRDEV_op);
    // 添加字符设备至cdev_map散列表中以便系统管理
    ret = cdev_add(&LED_newchrdev.cdev, LED_newchrdev.devid, LED_NEWCHRDEV_COUNT);
    if (ret < 0)
    {
        printk("LED_NEWCHRDEV_init: cdev_add failed!\n");
        goto cdev_add_failed;
    }
    // 在VFS中创建设备节点  /sys/class/${CLASS_NAME}
    //  创建设备类
    LED_newchrdev.pclass = class_create(THIS_MODULE, LED_NEWCHRDEV_NAME);
    if (IS_ERR(LED_newchrdev.pclass))
    {
        ret = PTR_ERR(LED_newchrdev.pclass);
        goto class_create_failed;
    }
    // 在设备类下创建设备结点 /sys/class/${CLASS_NAME}/${DEVICE_NAME}/和/dev/${DEVICE_NAME}
    LED_newchrdev.pdevice = device_create(LED_newchrdev.pclass, NULL, LED_newchrdev.devid, NULL, LED_NEWCHRDEV_NAME);
    if (IS_ERR(LED_newchrdev.pdevice))
    {
        ret = PTR_ERR(LED_newchrdev.pdevice);
        goto device_create_failed;
    }
    return 0;

device_create_failed:
    // 销毁设备类
    class_destroy(LED_newchrdev.pclass);
class_create_failed:
    // 删除字符设备
    cdev_del(&LED_newchrdev.cdev);
cdev_add_failed:
    // 注销设备号
    unregister_chrdev_region(LED_newchrdev.devid, LED_NEWCHRDEV_COUNT);
register_chrdev_failed:
    return ret;
}
static void __exit LED_NEWCHRDEV_exit(void)
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
    device_destroy(LED_newchrdev.pclass, LED_newchrdev.devid);
    // 销毁类
    class_destroy(LED_newchrdev.pclass);
    // 删除字符设备
    cdev_del(&LED_newchrdev.cdev);
    // 注销设备号
    unregister_chrdev_region(LED_newchrdev.devid, LED_NEWCHRDEV_COUNT);
    printk("LED_NEWCHRDEVLED_exit\n");
}
module_init(LED_NEWCHRDEV_init);
module_exit(LED_NEWCHRDEV_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("liu");