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
#define REGISTER_LENGTH 4

#define LED_PLATFORM_DEVICE_NAME "LED_platform_match_name"
#define LED_PLATFORM_DEVICE_COUNT 1

struct resource LED_resources[RESOURCE_NUM] =
    {
        [0] = {
            .start = CCM_CCGR1_BASE,
            .end = CCM_CCGR1_BASE + REGISTER_LENGTH - 1,
            .flags = IORESOURCE_MEM,
        },
        [1] = {
            .start = SW_MUX_GPIO1_IO03_BASE,
            .end = SW_MUX_GPIO1_IO03_BASE + REGISTER_LENGTH - 1,
            .flags = IORESOURCE_MEM,
        },
        [2] = {
            .start = SW_PAD_GPIO1_IO03_BASE,
            .end = SW_PAD_GPIO1_IO03_BASE + REGISTER_LENGTH - 1,
            .flags = IORESOURCE_MEM,
        },
        [3] = {
            .start = GPIO1_DR_BASE,
            .end = GPIO1_DR_BASE + REGISTER_LENGTH - 1,
            .flags = IORESOURCE_MEM,
        },
        [4] = {
            .start = GPIO1_GDIR_BASE,
            .end = GPIO1_GDIR_BASE + REGISTER_LENGTH - 1,
            .flags = IORESOURCE_MEM,
        },
};
// 卸载设备时，设备的release先于exit函数执行
// 匹配到驱动时，先卸载设备会先执行驱动的remove函数再执行设备的release和exit函数
void LED_platform_device_release(struct device *dev)
{
    printk("platform_device_release\n");
}

struct platform_device LED_platform_device = {
    .name = LED_PLATFORM_DEVICE_NAME,
    .id = -1, // 此设备无id
    .dev = {
        .release = LED_platform_device_release,
    },
    .resource = LED_resources,
    .num_resources = ARRAY_SIZE(LED_resources),
};

static int __init LED_PLATFORM_DEVICE_init(void)
{
    printk("LED_PLATFORM_DEVICE_init\n");
    return platform_device_register(&LED_platform_device);
}
static void __exit LED_PLATFORM_DEVICE_exit(void)
{
    platform_device_unregister(&LED_platform_device);
    printk("LED_PLATFORM_DEVICE_exit\n");
}
module_init(LED_PLATFORM_DEVICE_init);
module_exit(LED_PLATFORM_DEVICE_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("liu");