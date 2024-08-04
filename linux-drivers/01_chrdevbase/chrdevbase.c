#include <linux/types.h>
#include <linux/delay.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/ide.h>
#include <linux/uaccess.h>
static unsigned int major;
static char chrdevbase_name[20]="liu's chrdevbase";
static char read_buf[50];
static char write_buf[50];
static char driver_data[20]="kernel driver data";
int chrdevbase_open (struct inode *inode, struct file *f)
{
    printk("chrdevbase_open\n");
    return 0;
}
int chrdevbase_close (struct inode *inode, struct file *f)
{
    printk("chrdevbase_close\n");
    return 0;
}
ssize_t chrdevbase_read (struct file *f, char __user *buf, size_t count, loff_t *pos)
{
    int ret;
    memcpy(read_buf,driver_data,sizeof(driver_data));
    ret=copy_to_user(buf,read_buf,count);
    if(ret!=0)
    {
        printk("chrdevbase_read: kernel send data failed!\n");
    }else{
        printk("chrdevbase_read: kernel send data success!\n");
    }
    printk("chrdevbase_read\n");
    return count;
}
ssize_t chrdevbase_write (struct file *f, const char __user *buf, size_t count, loff_t *pos)
{
    int ret;
    ret=copy_from_user(write_buf,buf,count);
    if(ret!=0)
    {
        printk("chrdevbase_write: kernel recv data failed!\n");
    }else{
        printk("chrdevbase_write: kernel recv data=%s!\n",write_buf);
    }
    printk("chrdevbase_write\n");
    return count;
}
static const struct file_operations chrdevbase_op={
    .owner=THIS_MODULE,
    .open=chrdevbase_open,
    .release=chrdevbase_close,
    .write=chrdevbase_write,
    .read=chrdevbase_read
};
static int __init chrdevbase_init(void)
{ 
    major=register_chrdev(0,chrdevbase_name,&chrdevbase_op);//major=0,自动分配
    if(major<0)
    {
        printk("ERROR! chrdevbase_init major=%d\n",major);
        return -1;
    }
    printk("chrdevbase_init major=%d\n",major);
    return 0;
}
static void __exit chrdevbase_exit(void)
{
    unregister_chrdev(major,chrdevbase_name);
    printk("chrdevbase_exit major=%d\n",major);
}
module_init(chrdevbase_init);
module_exit(chrdevbase_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("liu");