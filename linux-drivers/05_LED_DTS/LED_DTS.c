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
// /* 寄存器物理地址 */
// #define CCM_CCGR1_BASE (0X020C406C)
// #define SW_MUX_GPIO1_IO03_BASE (0X020E0068)
// #define SW_PAD_GPIO1_IO03_BASE (0X020E02F4)
// #define GPIO1_DR_BASE (0X0209C000)
// #define GPIO1_GDIR_BASE (0X0209C004)

#define LED_ON 1
#define LED_OFF 0

#define LED_DTS_NAME "LED_DTS"
#define LED_DTS_COUNT 1
// 映射后的虚拟地址
static void __iomem *IMX6U_CCM_CCGR1;
static void __iomem *SW_MUX_GPIO1_IO03;
static void __iomem *SW_PAD_GPIO1_IO03;
static void __iomem *GPIO1_DR;
static void __iomem *GPIO1_GDIR;

// LED_DTS设备结构体
struct LED
{
    dev_t devid; // 设备号
    uint major;  // 主设备号
    uint minor;  // 次设备号
    struct cdev cdev;
    struct class *pclass;
    struct device *pdevice;
    struct device_node *pdevnode;
};
static struct LED LED_DTS;
int LED_DTS_open(struct inode *inode, struct file *f)
{
    f->private_data = &LED_DTS; // 设置私有数据
    return 0;
}
int LED_DTS_close(struct inode *inode, struct file *f)
{
    return 0;
}
ssize_t LED_DTS_write(struct file *f, const char __user *buf, size_t count, loff_t *pos)
{
    int ret;
    u32 val;
    unsigned char LED_Cmd = 0;
    ret = copy_from_user(&LED_Cmd, buf, sizeof(LED_Cmd));
    if (ret < 0)
    {
        printk("LED_DTS_write: copy_from_user failed!\n");
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
static const struct file_operations LED_DTS_op = {
    .owner = THIS_MODULE,
    .write = LED_DTS_write,
    .open = LED_DTS_open,
    .release = LED_DTS_close,
};

static int __init LED_DTS_init(void)
{
    u32 val;
    int ret;
    u8 i;
    u32 reg_address[10] = {0};
    char *status;
    char *compatible;
    LED_DTS.pdevnode = of_find_node_by_path("/led@liu");
    if (LED_DTS.pdevnode == NULL)
    {
        printk("LED_DTS_init: of_find_node_by_path failed!\n");
        ret = -1;
        goto failed;
    }
    ret = of_property_read_u32_array(LED_DTS.pdevnode, "reg", reg_address, 10);
    if (of_property_read_u32_array < 0)
    {
        printk("LED_DTS_init: of_find_node_by_path failed!\n");
        goto failed;
    }
    for (i = 0; i < sizeof(reg_address) / sizeof(u32); i++)
    {
        printk("%#X ", reg_address[i]);
    }
    printk("\n");

    ret = of_property_read_string(LED_DTS.pdevnode, "status", (const char **)&status);
    ret = of_property_read_string(LED_DTS.pdevnode, "compatible", (const char **)&compatible);
    if (ret < 0)
    {
        printk("LED_DTS_init: of_property_read_string failed!\n");
        goto failed;
    }
    printk("status=%s\n", status);
    printk("compatible=%s\n", compatible);
#if 0
    // 寄存器地址映射
    IMX6U_CCM_CCGR1=ioremap(reg_address[0],reg_address[1]);
    SW_MUX_GPIO1_IO03 = ioremap(reg_address[2], reg_address[3]);
    SW_PAD_GPIO1_IO03 = ioremap(reg_address[4], reg_address[5]);
    GPIO1_DR = ioremap(reg_address[6], reg_address[7]);
    GPIO1_GDIR = ioremap(reg_address[8], reg_address[9]);
#else
    IMX6U_CCM_CCGR1 = of_iomap(LED_DTS.pdevnode, 0);
    SW_MUX_GPIO1_IO03 = of_iomap(LED_DTS.pdevnode, 1);
    SW_PAD_GPIO1_IO03 = of_iomap(LED_DTS.pdevnode, 2);
    GPIO1_DR = of_iomap(LED_DTS.pdevnode, 3);
    GPIO1_GDIR = of_iomap(LED_DTS.pdevnode, 4);
#endif
    // 使能 GPIO1 时钟
    val = readl(IMX6U_CCM_CCGR1);
    val &= ~(3 << 26); /* 清除以前的设置 */
    val |= (3 << 26);  /* 设置新值 */
    writel(val, IMX6U_CCM_CCGR1);

    // 设置 GPIO1_IO03 的复用功能，将其复用为GPIO1_IO03，最后设置IO属性
    writel(5, SW_MUX_GPIO1_IO03);
    // 寄存器 SW_PAD_GPIO1_IO03 设置 IO 属性
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
    if (LED_DTS.major)
    {
        LED_DTS.devid = MKDEV(LED_DTS.major, LED_DTS.minor);
        ret = register_chrdev_region(LED_DTS.devid, LED_DTS_COUNT, LED_DTS_NAME);
        if (ret < 0)
        {
            printk("LED_DTS_init: register_chrdev_region failed!\n");
            goto register_chrdev_failed;
        }
    }
    else
    {
        ret = alloc_chrdev_region(&LED_DTS.devid, 0, LED_DTS_COUNT, LED_DTS_NAME);
        if (ret < 0)
        {
            printk("LED_DTS_init: alloc_chrdev_region failed!\n");
            goto register_chrdev_failed;
        }
        LED_DTS.major = MAJOR(LED_DTS.devid);
        LED_DTS.minor = MINOR(LED_DTS.devid);
    }

    printk("LED_DTS_init\nLED_DTS_devid=%d LED_DTS_major=%d\
    LED_DTS_minor=%d\n",
           LED_DTS.devid, LED_DTS.major, LED_DTS.minor);
    // 关联字符设备结构体cdev与文件操作结构体file_operations
    cdev_init(&LED_DTS.cdev, &LED_DTS_op);
    // 添加字符设备至cdev_map散列表中以便系统管理
    ret = cdev_add(&LED_DTS.cdev, LED_DTS.devid, LED_DTS_COUNT);
    if (ret < 0)
    {
        printk("LED_DTS_init: cdev_add failed!\n");
        goto cdev_add_failed;
    }
    // 在VFS中创建设备节点  /sys/class/${CLASS_NAME}/
    //  创建设备类
    LED_DTS.pclass = class_create(THIS_MODULE, LED_DTS_NAME);
    if (IS_ERR(LED_DTS.pclass))
    {
        ret = PTR_ERR(LED_DTS.pclass);
        goto class_create_failed;
    }
    // 在设备类下创建设备结点 /sys/class/${CLASS_NAME}/${DEVICE_NAME}/和/dev/${DEVICE_NAME}
    LED_DTS.pdevice = device_create(LED_DTS.pclass, NULL, LED_DTS.devid, NULL, LED_DTS_NAME);
    if (IS_ERR(LED_DTS.pdevice))
    {
        ret = PTR_ERR(LED_DTS.pdevice);
        goto device_create_failed;
    }
    return 0;

device_create_failed:
    // 销毁设备类
    class_destroy(LED_DTS.pclass);
class_create_failed:
    // 删除字符设备
    cdev_del(&LED_DTS.cdev);
cdev_add_failed:
    // 注销设备号
    unregister_chrdev_region(LED_DTS.devid, LED_DTS_COUNT);
register_chrdev_failed:
    printk("register_chrdev_failed\n");
failed:
    return ret;
}
static void __exit LED_DTS_exit(void)
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
    device_destroy(LED_DTS.pclass, LED_DTS.devid);
    // 销毁类
    class_destroy(LED_DTS.pclass);
    // 删除字符设备
    cdev_del(&LED_DTS.cdev);
    // 注销设备号
    unregister_chrdev_region(LED_DTS.devid, LED_DTS_COUNT);
    printk("LED_DTSLED_exit\n");
}
module_init(LED_DTS_init);
module_exit(LED_DTS_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("liu");