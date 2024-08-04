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
#include <linux/input.h>
#include <linux/i2c.h>
#include <linux/spi/spi.h>
#include "icm20608.h"
#define ICM20608_NAME "icm20608"
#define ICM20608_COUNT 1
struct ICM20608
{
    dev_t devid; // 设备号
    uint major;  // 主设备号
    uint minor;  // 次设备号
    struct cdev cdev;
    struct class *pclass;
    struct device *pdevice;
    struct device_node *pnode;
    void *private_data;
    // int cs_gpio;            // 设备树中的片选引脚（软件片选） SPI控制器自动控制
    signed int gyro_x_adc;  // 陀螺仪X轴原始值
    signed int gyro_y_adc;  // 陀螺仪Y轴原始值
    signed int gyro_z_adc;  // 陀螺仪Z轴原始值
    signed int accel_x_adc; // 加速度计X轴原始值
    signed int accel_y_adc; // 加速度计Y轴原始值
    signed int accel_z_adc; // 加速度计Z轴原始值
    signed int temp_adc;    // 温度原始值
};
static struct ICM20608 icm20608;

static int icm20608_read_regs(struct ICM20608 *icm20608, u8 reg, void *buf, u8 length)
{
    int ret;
    unsigned char txdata;
    unsigned char *rxdata;
    struct spi_message msg;
    struct spi_transfer *transfer;
    struct spi_device *spi_device = (struct spi_device *)icm20608->private_data;
    transfer = kzalloc(sizeof(struct spi_transfer), GFP_KERNEL);
    if (transfer == NULL)
    {
        return -ENOMEM;
    }
    rxdata = kzalloc(sizeof(unsigned char) * length, GFP_KERNEL);
    if (rxdata == NULL)
    {
        kfree(transfer);
        return -ENOMEM;
    }

    txdata = reg | 0x80; // 主机输出，icm20608输入
    transfer->tx_buf = &txdata;
    transfer->rx_buf = rxdata;
    transfer->len = length + 1;
    spi_message_init(&msg);
    spi_message_add_tail(transfer, &msg);
    ret = spi_sync(spi_device, &msg);
    memcpy(buf, rxdata + 1, length);
    kfree(rxdata);
    kfree(transfer);
    return ret;
}
static unsigned char icm20608_read_reg(struct ICM20608 *icm20608, u8 reg)
{
    u8 data = 0;
    icm20608_read_regs(icm20608, reg, &data, 1);
    return data;
}
static int icm20608_write_regs(struct ICM20608 *icm20608, u8 reg, const void *buf, u8 length)
{
    int ret;
    unsigned char *txdata;
    struct spi_message msg;
    struct spi_transfer *transfer;
    struct spi_device *spi_device = (struct spi_device *)icm20608->private_data;
    transfer = kzalloc(sizeof(struct spi_transfer), GFP_KERNEL);
    if (transfer == NULL)
    {
        return -ENOMEM;
    }
    txdata = kzalloc(sizeof(unsigned char) + length, GFP_KERNEL);
    if (txdata == NULL)
    {
        kfree(transfer);
        return -ENOMEM;
    }
    *txdata = reg & ~0x80; // // 主机输出，icm20608输入
    memcpy(txdata + 1, buf, length);
    transfer->tx_buf = txdata;
    transfer->len = length + 1;
    spi_message_init(&msg);
    spi_message_add_tail(transfer, &msg);
    ret = spi_sync(spi_device, &msg);

    kfree(transfer);
    kfree(txdata);
    return ret;
}
static int icm20608_write_reg(struct ICM20608 *icm20608, u8 reg, u8 data)
{
    u8 buf = data;
    return icm20608_write_regs(icm20608, reg, &buf, 1);
}
void icm20608_read_data(struct ICM20608 *icm20608)
{
    unsigned char data[14] = {0};
    icm20608_read_regs(icm20608, ICM20_ACCEL_XOUT_H, data, 14);

    icm20608->accel_x_adc = (signed short)((data[0] << 8) | data[1]);
    icm20608->accel_y_adc = (signed short)((data[2] << 8) | data[3]);
    icm20608->accel_z_adc = (signed short)((data[4] << 8) | data[5]);
    icm20608->temp_adc = (signed short)((data[6] << 8) | data[7]);
    icm20608->gyro_x_adc = (signed short)((data[8] << 8) | data[9]);
    icm20608->gyro_y_adc = (signed short)((data[10] << 8) | data[11]);
    icm20608->gyro_z_adc = (signed short)((data[12] << 8) | data[13]);
}
int ICM20608_open(struct inode *inode, struct file *f)
{
    f->private_data = &icm20608;

    printk("ICM20608_open\n");
    return 0;
}
int ICM20608_close(struct inode *inode, struct file *f)
{
    f->private_data = &icm20608;

    printk("ICM20608_close\n");
    return 0;
}

ssize_t ICM20608_read(struct file *f, char __user *buf, size_t count, loff_t *pos)
{
    int ret;
    signed int data[7];
    struct ICM20608 *icm20608 = (struct ICM20608 *)f->private_data;

    icm20608_read_data(icm20608);
    data[0] = icm20608->gyro_x_adc;
    data[1] = icm20608->gyro_y_adc;
    data[2] = icm20608->gyro_z_adc;
    data[3] = icm20608->accel_x_adc;
    data[4] = icm20608->accel_y_adc;
    data[5] = icm20608->accel_z_adc;
    data[6] = icm20608->temp_adc;
    ret = copy_to_user(buf, data, sizeof(data));
    return ret;
}
static const struct file_operations ICM20608_op = {
    .owner = THIS_MODULE,
    .read = ICM20608_read,
    .open = ICM20608_open,
    .release = ICM20608_close,
};
void icm20608_reg_init(struct ICM20608 *icm20608)
{
    u8 value;
    int ret;
    ret = icm20608_write_reg(icm20608, ICM20_PWR_MGMT_1, 0x80);
    mdelay(50);
    ret = icm20608_write_reg(icm20608, ICM20_PWR_MGMT_1, 0x01);
    mdelay(50);
    icm20608_write_reg(icm20608, ICM20_SMPLRT_DIV, 0x00);    // 输出速率是内部采样率
    icm20608_write_reg(icm20608, ICM20_GYRO_CONFIG, 0x18);   // 陀螺仪±2000dps量程
    icm20608_write_reg(icm20608, ICM20_ACCEL_CONFIG, 0x18);  // 加速度计±16G量程
    icm20608_write_reg(icm20608, ICM20_CONFIG, 0x04);        // 陀螺仪低通滤波BW=20Hz
    icm20608_write_reg(icm20608, ICM20_ACCEL_CONFIG2, 0x04); // 加速度计低通滤波BW=21.2Hz
    icm20608_write_reg(icm20608, ICM20_PWR_MGMT_2, 0x00);    // 打开加速度计和陀螺仪所有轴
    icm20608_write_reg(icm20608, ICM20_LP_MODE_CFG, 0x00);   // 关闭低功耗
    icm20608_write_reg(icm20608, ICM20_FIFO_EN, 0x00);       // 关闭FIFO
    value = icm20608_read_reg(icm20608, ICM20_PWR_MGMT_1);
    printk("ICM20_PWR_MGMT_1 value=%#X\n", value);
    value = icm20608_read_reg(icm20608, ICM20_PWR_MGMT_2);
    printk("ICM20_PWR_MGMT_2 value=%#X\n", value);
    value = icm20608_read_reg(icm20608, ICM20_WHO_AM_I);
    printk("ICM20608 ID = %#X\r\n", value);
}

int icm20608_driver_probe(struct spi_device *spi)
{
    int ret;
    printk("icm20608_driver_probe\n");

    // 搭建字符设备驱动框架
    // 向linux系统申请设备编号
    if (icm20608.major)
    {
        icm20608.devid = MKDEV(icm20608.major, icm20608.minor);
        ret = register_chrdev_region(icm20608.devid, ICM20608_COUNT, ICM20608_NAME);
        if (ret < 0)
        {
            printk("ICM20608_init: register_chrdev_region failed!\n");
            goto register_chrdev_failed;
        }
    }
    else
    {
        ret = alloc_chrdev_region(&icm20608.devid, 0, ICM20608_COUNT, ICM20608_NAME);
        if (ret < 0)
        {
            printk("ICM20608_init: alloc_chrdev_region failed!\n");
            goto register_chrdev_failed;
        }
        icm20608.major = MAJOR(icm20608.devid);
        icm20608.minor = MINOR(icm20608.devid);
    }
    printk("icm20608.major=%d  icm20608.minor=%d\n", icm20608.major, icm20608.minor);
    // 关联字符设备结构体cdev与文件操作结构体file_operations
    cdev_init(&icm20608.cdev, &ICM20608_op);
    // 添加字符设备至cdev_map散列表中以便系统管理
    ret = cdev_add(&icm20608.cdev, icm20608.devid, ICM20608_COUNT);
    if (ret < 0)
    {
        printk("ICM20608_init: cdev_add failed!\n");
        goto cdev_add_failed;
    }
    // 在VFS中创建设备节点  /sys/class/${CLASS_NAME}/
    //  创建设备类
    icm20608.pclass = class_create(THIS_MODULE, ICM20608_NAME);
    if (IS_ERR(icm20608.pclass))
    {
        ret = PTR_ERR(icm20608.pclass);
        goto class_create_failed;
    }
    // 在VFS中创建设备节点  /sys/class/${CLASS_NAME}/
    icm20608.pdevice = device_create(icm20608.pclass, NULL, icm20608.devid, NULL, ICM20608_NAME);
    if (IS_ERR(icm20608.pdevice))
    {
        ret = PTR_ERR(icm20608.pdevice);
        goto device_create_failed;
    }
    spi->mode = SPI_MODE_0;
    spi_setup(spi);
    icm20608.private_data = spi;
    icm20608_reg_init(&icm20608);
    return 0;
device_create_failed:
    // 销毁设备类
    class_destroy(icm20608.pclass);
class_create_failed:
    // 删除字符设备
    cdev_del(&icm20608.cdev);
cdev_add_failed:
    // 注销设备号
    unregister_chrdev_region(icm20608.devid, ICM20608_COUNT);
register_chrdev_failed:
    return ret;
}
int icm20608_driver_remove(struct spi_device *spi)
{
    // 销毁设备
    device_destroy(icm20608.pclass, icm20608.devid);
    // 销毁类
    class_destroy(icm20608.pclass);
    // 删除字符设备
    cdev_del(&icm20608.cdev);
    // 注销设备号
    unregister_chrdev_region(icm20608.devid, ICM20608_COUNT);
    printk("icm20608_driver_remove\n");
    return 0;
}
struct of_device_id icm20608_of_match[] = {
    {.compatible = "liu,icm20608"},
    {},
};
struct spi_device_id icm20608_id[] = {
    {"liu,icm20608", 0},
    {},
};
static struct spi_driver icm20608_driver = {
    .driver = {
        .name = "icm20608-liu",
        .owner = THIS_MODULE,
        .of_match_table = icm20608_of_match,
    },
    .id_table = icm20608_id,
    .probe = icm20608_driver_probe,
    .remove = icm20608_driver_remove,
};
static int __init SPI_ICM20608_init(void)
{
    int ret;
    printk("SPI_ICM20608_init\n");
    ret = spi_register_driver(&icm20608_driver);
    return ret;
}

static void __exit SPI_ICM20608_exit(void)
{
    spi_unregister_driver(&icm20608_driver);
    printk("SPI_ICM20608_exit\n");
}

module_init(SPI_ICM20608_init);
module_exit(SPI_ICM20608_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("liu");