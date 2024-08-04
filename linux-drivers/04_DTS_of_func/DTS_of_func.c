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
#include "linux/slab.h"
#if 0
backlight {
    compatible = "pwm-backlight";
    pwms = <&pwm1 0 5000000>;
    brightness-levels = <0 4 8 16 32 64 128 255>;
    default-brightness-level = <6>;
    status = "okay";
};
#endif


static int __init backlight_init(void)
{
    u32 val;
    int ret;
    u8 i;
    struct device_node* dev_node=NULL;
    char* status;
    char* compatible=NULL;
    u32 default_brightness_level;
    u32 elemsize;
    dev_node=of_find_node_by_path("/backlight");
    if(dev_node==NULL)
    {
        printk(KERN_NOTICE"backlight_init: of_find_node_by_path failed!\n");
        return -EINVAL;
    }

    
    ret=of_property_read_u32(dev_node,"default-brightness-level",&default_brightness_level);
    if(ret<0)
    {
        printk(KERN_NOTICE"backlight_init: of_property_read_u32 failed!\n");
        return -EINVAL;
    }
    printk("default-brightness-level=%u\n",default_brightness_level);

    ret=of_property_read_string(dev_node,"status",(const char**)&status);//指针直接指向
    if(ret<0)
    {
        printk(KERN_NOTICE"backlight_init: of_property_read_string failed!\n");
        return -EINVAL;
    }
    printk("status=%s\n",status);


    elemsize=of_property_count_elems_of_size(dev_node,"brightness-levels",sizeof(u32));
    if(elemsize<0)
    {
        printk(KERN_NOTICE"backlight_init: of_property_count_elems_of_size failed!\n");
        return -EINVAL;
    }
    printk("elemsize=%d\n",elemsize);
    for(i=0;i<elemsize;i++)
    {
        ret=of_property_read_u32_index(dev_node,"brightness-levels",i,&val);
        if(ret<0)
        {
            printk(KERN_NOTICE"backlight_init: of_property_read_u32_index failed!\n");
            return -EINVAL;
        }
        printk("brightness-levels[%d]=%u\n",i,val);
    }


    compatible=(char*)kmalloc(20,GFP_KERNEL);
    memset(compatible,0,20);
    if(compatible==NULL)
    {
        printk(KERN_NOTICE"backlight_init: kmalloc failed!");
        return -EINVAL;
    }
    ret=of_property_read_u8_array(dev_node,"compatible",(u8*)compatible,14);
    if(ret<0)
    {
        printk(KERN_NOTICE"backlight_init: of_property_read_string failed!\n");
        return -EINVAL;
    }
    printk("compatible=%s\n",compatible);
    kfree(compatible);
    printk(KERN_NOTICE"backlight_init\n");
    return 0;

}
static void __exit backlight_exit(void)
{
    printk("backlight_exit\n");
}
module_init(backlight_init);
module_exit(backlight_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("liu");