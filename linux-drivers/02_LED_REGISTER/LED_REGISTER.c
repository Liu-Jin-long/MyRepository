#include <linux/types.h>
#include <linux/delay.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/ide.h>
#include <linux/uaccess.h>
#include <linux/io.h>

/* 寄存器物理地址 */
#define CCM_CCGR1_BASE (0X020C406C)
#define SW_MUX_GPIO1_IO03_BASE (0X020E0068)
#define SW_PAD_GPIO1_IO03_BASE (0X020E02F4)
#define GPIO1_DR_BASE (0X0209C000)
#define GPIO1_GDIR_BASE (0X0209C004)

#define LED_ON 1
#define LED_OFF 0

#define LED_NAME "LED"
static int LED_major;

// 映射后的虚拟地址
static void __iomem *IMX6U_CCM_CCGR1;
static void __iomem *SW_MUX_GPIO1_IO03;
static void __iomem *SW_PAD_GPIO1_IO03;
static void __iomem *GPIO1_DR;
static void __iomem *GPIO1_GDIR;
int LED_open (struct inode *inode, struct file *f)
{
    return 0;
}
int LED_close (struct inode *inode, struct file *f)
{
    return 0;
}
ssize_t LED_write (struct file *f, const char __user *buf, size_t count, loff_t *pos)
{
    int ret;
    u32 val;
    unsigned char LED_Cmd=0;
    ret=copy_from_user(&LED_Cmd,buf,sizeof(LED_Cmd));
    if(ret<0)
    {
        printk("LED_write: copy_from_user failed!\n");
        return -EFAULT;
    }
    if(LED_Cmd==LED_OFF)
    {
        val = readl(GPIO1_DR);
        val |= (1 << 3);
        writel(val, GPIO1_DR);

    }else if(LED_Cmd==LED_ON)
    {
        val = readl(GPIO1_DR);
        val &= ~(1 << 3);
        writel(val, GPIO1_DR);
    }
    return 0;

}
static const struct file_operations LED_op={
    .owner=THIS_MODULE,
    .write=LED_write,
    .open=LED_open,
    .release=LED_close,
};

static int __init LED_init(void)
{
    u32 val;
    // 寄存器地址映射
    IMX6U_CCM_CCGR1=ioremap(CCM_CCGR1_BASE,4);
    SW_MUX_GPIO1_IO03 = ioremap(SW_MUX_GPIO1_IO03_BASE, 4);
    SW_PAD_GPIO1_IO03 = ioremap(SW_PAD_GPIO1_IO03_BASE, 4);
    GPIO1_DR = ioremap(GPIO1_DR_BASE, 4);
    GPIO1_GDIR = ioremap(GPIO1_GDIR_BASE, 4);

    // 使能 GPIO1 时钟
    val = readl(IMX6U_CCM_CCGR1);
    val &= ~(3 << 26); /* 清除以前的设置 */
    val |= (3 << 26); /* 设置新值 */
    writel(val, IMX6U_CCM_CCGR1);

    // 设置 GPIO1_IO03 的复用功能，将其复用为GPIO1_IO03，最后设置IO属性
    writel(5, SW_MUX_GPIO1_IO03);
    // 寄存器 SW_PAD_GPIO1_IO03 设置 IO 属性 
    writel(0x10B0, SW_PAD_GPIO1_IO03);

    // 设置 GPIO1_IO03 为输出功能
    val = readl(GPIO1_GDIR);
    val &= ~(1 << 3); /* 清除以前的设置 */
    val |= (1 << 3); /* 设置为输出 */
    writel(val, GPIO1_GDIR);

    // 默认关闭 LED
    val = readl(GPIO1_DR);
    val |= (1 << 3);
    writel(val, GPIO1_DR);
    // 注册字符设备驱动
    LED_major=register_chrdev(0,LED_NAME,&LED_op);
    if(LED_major<0)
    {
        printk("LED_init failed! LED_major=%d\n",LED_major);
    }
    printk("LED_init LED_major=%d\n",LED_major);
    return 0;
}
static void __exit LED_exit(void)
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


    unregister_chrdev(LED_major,LED_NAME);
    printk("LED_exit\n");
}
module_init(LED_init);
module_exit(LED_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("liu");