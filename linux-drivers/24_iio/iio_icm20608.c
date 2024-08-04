#include <linux/spi/spi.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/ide.h>
#include <linux/errno.h>
#include <linux/platform_device.h>

#include <linux/gpio.h>
#include <linux/device.h>
#include <asm/uaccess.h>
#include <linux/cdev.h>
#include <linux/regmap.h>
#include <linux/iio/iio.h>
#include <linux/iio/sysfs.h>
#include <linux/iio/buffer.h>
#include <linux/iio/trigger.h>
#include <linux/iio/triggered_buffer.h>
#include <linux/iio/trigger_consumer.h>
#include <linux/unaligned/be_byteshift.h>
#include "icm20608.h"

#define ICM20608_NAME "icm20608"
#define ICM20608_TEMP_OFFSET 0
#define ICM20608_TEMP_SCALE 326800000
#define ICM20608_CHAN(_type, _channel2, _index)                         \
    {                                                                   \
        .type = _type,                                                  \
        .modified = 1, /*modified为1时，channel2是通道修饰符*/ \
        .channel2 = _channel2,                                          \
        .info_mask_shared_by_type = BIT(IIO_CHAN_INFO_SCALE),           \
        .info_mask_separate = BIT(IIO_CHAN_INFO_RAW) |                  \
                              BIT(IIO_CHAN_INFO_CALIBBIAS),             \
        .scan_index = _index,                                           \
        .scan_type = {                                                  \
            .sign = 's',                                                \
            .realbits = 16,                                             \
            .storagebits = 16,                                          \
            .shift = 0,                                                 \
            .endianness = IIO_BE,                                       \
        },                                                              \
    }

struct ICM20608
{
    struct spi_device *spi;
    struct regmap *regmap;
    struct regmap_config regmap_config;
    struct mutex lock;
};
// ICM20608的扫描元素，3轴加速度计、3轴陀螺仪、1路温度传感器，1路时间戳
enum inv_icm20608_scan
{
    INV_ICM20608_SCAN_ACCL_X,
    INV_ICM20608_SCAN_ACCL_Y,
    INV_ICM20608_SCAN_ACCL_Z,
    INV_ICM20608_SCAN_TEMP,
    INV_ICM20608_SCAN_GYRO_X,
    INV_ICM20608_SCAN_GYRO_Y,
    INV_ICM20608_SCAN_GYRO_Z,
    INV_ICM20608_SCAN_TIMESTAMP,
};
static const struct iio_chan_spec icm20608_channels[] = {
    /* 温度通道 */
    {
        .type = IIO_TEMP,
        .info_mask_separate = BIT(IIO_CHAN_INFO_RAW) | BIT(IIO_CHAN_INFO_OFFSET) | BIT(IIO_CHAN_INFO_SCALE),
        .scan_index = INV_ICM20608_SCAN_TEMP,
        .scan_type = {
            .sign = 's',
            .realbits = 16,
            .storagebits = 16,
            .shift = 0,
            .endianness = IIO_BE,
        },
    },
    ICM20608_CHAN(IIO_ANGL_VEL, IIO_MOD_X, INV_ICM20608_SCAN_GYRO_X),
    ICM20608_CHAN(IIO_ANGL_VEL, IIO_MOD_Y, INV_ICM20608_SCAN_GYRO_Y),
    ICM20608_CHAN(IIO_ANGL_VEL, IIO_MOD_Z, INV_ICM20608_SCAN_GYRO_Z),

    ICM20608_CHAN(IIO_ACCEL, IIO_MOD_Y, INV_ICM20608_SCAN_ACCL_Y),
    ICM20608_CHAN(IIO_ACCEL, IIO_MOD_X, INV_ICM20608_SCAN_ACCL_X),
    ICM20608_CHAN(IIO_ACCEL, IIO_MOD_Z, INV_ICM20608_SCAN_ACCL_Z),

};
// icm20608陀螺仪分辨率，对应 250、500、1000、2000
// 计算方法：以正负250度量程为例，500/2^16=0.007629，扩大1000000倍即7629
static const int gyro_scale_icm20608[] = {7629, 15258, 30517, 61035};
// icm20608 加速度计分辨率，对应 2、4、8、16
// 计算方法：以正负2g量程为例，4/2^16=0.000061035，扩大1000000000倍，即61035
static const int accel_scale_icm20608[] = {61035, 122070, 244140, 488281};
static unsigned char icm20608_read_reg(struct ICM20608 *icm20608, u8 reg)
{
    unsigned int data = 0;
    regmap_read(icm20608->regmap, reg, &data);
    return (unsigned char)data;
}

static int icm20608_write_reg(struct ICM20608 *icm20608, u8 reg, u8 data)
{
    return regmap_write(icm20608->regmap, reg, (unsigned int)data);
}
static int icm20608_sensor_show(struct ICM20608 *dev, int reg, int axis, int *val)
{
    int index, result;
    __be16 d;

    index = (axis - IIO_MOD_X) * 2;
    result = regmap_bulk_read(dev->regmap, reg + index, (u8 *)&d, 2);
    if (result)
        return -EINVAL;
    *val = (short)be16_to_cpup(&d); // 大端字节序转主机字节序（通常是CPU所使用的字节序）

    return IIO_VAL_INT;
}
static int icm20608_read_channel_data(struct iio_dev *indio_dev, struct iio_chan_spec const *chan, int *val)
{
    int ret;
    struct ICM20608 *icm20608 = iio_priv(indio_dev);
    switch (chan->type)
    {
    case IIO_ANGL_VEL:                                                                // 读取陀螺仪数据
        ret = icm20608_sensor_show(icm20608, ICM20_GYRO_XOUT_H, chan->channel2, val); // channel2指明X、Y、Z轴
        break;
    case IIO_ACCEL:                                                                    // 读取加速度计数据
        ret = icm20608_sensor_show(icm20608, ICM20_ACCEL_XOUT_H, chan->channel2, val); // channel2指明X、Y、Z轴
        break;
    case IIO_TEMP: // 读取温度
        ret = icm20608_sensor_show(icm20608, ICM20_TEMP_OUT_H, IIO_MOD_X, val);
        break;
    default:
        ret = -EINVAL;
        break;
    }
    return ret;
}

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
// 当读取sysfs中的文件时会执行此函数，从传感器里面读取各种数据上传给应用
// 如果是浮点数，val是整数部分，val2是小数部分
static int icm20608_read_raw(struct iio_dev *indio_dev, struct iio_chan_spec const *chan, int *val, int *val2, long mask)
{
    int ret;
    unsigned char regdata;
    struct ICM20608 *icm20608 = iio_priv(indio_dev);
    // printk("icm20608_read_raw\n");
    switch (mask)
    {
    case IIO_CHAN_INFO_RAW: // 读取ICM20608的加速度计、陀螺仪和温度传感器的原始数据
        mutex_lock(&icm20608->lock);
        ret = icm20608_read_channel_data(indio_dev, chan, val);
        mutex_unlock(&icm20608->lock);
        break;
    case IIO_CHAN_INFO_SCALE: // 读取ICM20608的加速度计、陀螺仪和温度传感器的量程分辨率
        switch (chan->type)
        {
        case IIO_ANGL_VEL: // 陀螺仪
            mutex_lock(&icm20608->lock);
            regdata = (icm20608_read_reg(icm20608, ICM20_GYRO_CONFIG) & 0X18) >> 3;
            *val = 0;
            *val2 = gyro_scale_icm20608[regdata];
            mutex_unlock(&icm20608->lock);
            ret = IIO_VAL_INT_PLUS_MICRO; // 数据格式val+val2/1000 000
            break;
        case IIO_ACCEL: // 加速度计
            mutex_lock(&icm20608->lock);
            regdata = (icm20608_read_reg(icm20608, ICM20_ACCEL_CONFIG) & 0X18) >> 3;
            *val = 0;
            *val2 = accel_scale_icm20608[regdata];
            mutex_unlock(&icm20608->lock);
            ret = IIO_VAL_INT_PLUS_NANO; // 数据格式val+val2/1000 000 000
            break;
        case IIO_TEMP: // 温度
            *val = ICM20608_TEMP_SCALE / 1000000;
            *val2 = ICM20608_TEMP_SCALE % 1000000;
            ret = IIO_VAL_INT_PLUS_MICRO; // 数据格式val+val2/1000 000
            break;
        default:
            ret = -EINVAL;
            break;
        }
        break;
    case IIO_CHAN_INFO_OFFSET: // 读取ICM20608温度传感器offset值
        if (chan->type == IIO_TEMP)
        {
            *val = ICM20608_TEMP_OFFSET;
            ret = IIO_VAL_INT;
        }
        else
        {
            ret = -EINVAL;
        }
        break;
    case IIO_CHAN_INFO_CALIBBIAS: // 读取ICM20608加速度计和陀螺仪校准值
        switch (chan->type)
        {
        case IIO_ANGL_VEL: // 陀螺仪的校准值
            mutex_lock(&icm20608->lock);
            ret = icm20608_sensor_show(icm20608, ICM20_XG_OFFS_USRH, chan->channel2, val);
            mutex_unlock(&icm20608->lock);
            break;
        case IIO_ACCEL: // 加速度计的校准值
            mutex_lock(&icm20608->lock);
            ret = icm20608_sensor_show(icm20608, ICM20_XA_OFFSET_H, chan->channel2, val);
            mutex_unlock(&icm20608->lock);
            break;
        default:
            ret = -EINVAL;
            break;
        }
    default:
        ret = -EINVAL;
        break;
    }
    return ret;
}
static int icm20608_sensor_set(struct ICM20608 *icm20608, int reg, int axis, int val)
{
    int index, result;
    __be16 d = cpu_to_be16(val);

    index = (axis - IIO_MOD_X) * 2;
    result = regmap_bulk_write(icm20608->regmap, reg + index, (u8 *)&d, 2);
    if (result < 0)
        return -EINVAL;
    return 0;
}
// 设置ICM20608的陀螺仪计量程(分辨率)
static int icm20608_write_gyro_scale(struct ICM20608 *icm20608, int val)
{
    int result, i;
    u8 d;
    for (i = 0; i < ARRAY_SIZE(gyro_scale_icm20608); i++)
    {
        if (gyro_scale_icm20608[i] == val)
        {
            d = (i << 3);
            result = regmap_write(icm20608->regmap, ICM20_GYRO_CONFIG, d);
            if (result)
                return result;
            return 0;
        }
    }
    return -EINVAL;
}
// 设置ICM20608的加速度计量程(分辨率)
static int icm20608_write_accel_scale(struct ICM20608 *icm20608, int val)
{
    int result, i;
    u8 d;

    for (i = 0; i < ARRAY_SIZE(accel_scale_icm20608); ++i)
    {
        if (accel_scale_icm20608[i] == val)
        {
            d = (i << 3);
            result = regmap_write(icm20608->regmap, ICM20_ACCEL_CONFIG, d);
            if (result)
                return result;
            return 0;
        }
    }
    return -EINVAL;
}
// 当写入sysfs中的文件时会执行此函数，将数据写入传感器的寄存器中
// 如果是浮点数，val是整数部分，val2是小数部分
static int icm20608_write_raw(struct iio_dev *indio_dev, struct iio_chan_spec const *chan, int val, int val2, long mask)
{
    int ret;
    struct ICM20608 *icm20608 = iio_priv(indio_dev);
    // printk("icm20608_write_raw\r\n");
    switch (mask)
    {
    case IIO_CHAN_INFO_SCALE: // 设置陀螺仪和加速度计的分辨率
        switch (chan->type)
        {
        case IIO_ANGL_VEL: // 陀螺仪分辨率
            mutex_lock(&icm20608->lock);
            ret = icm20608_write_gyro_scale(icm20608, val2);
            mutex_unlock(&icm20608->lock);
            break;
        case IIO_ACCEL: // 加速度计分辨率
            mutex_lock(&icm20608->lock);
            ret = icm20608_write_accel_scale(icm20608, val2);
            mutex_unlock(&icm20608->lock);
            break;
        default:
            ret = -EINVAL;
            break;
        }
        break;
    case IIO_CHAN_INFO_CALIBBIAS: // 设置陀螺仪和加速度计的校准值
        switch (chan->type)
        {
        case IIO_ANGL_VEL: // 陀螺仪校准值
            mutex_lock(&icm20608->lock);
            ret = icm20608_sensor_set(icm20608, ICM20_XG_OFFS_USRH, chan->channel2, val);
            mutex_unlock(&icm20608->lock);
            break;
        case IIO_ACCEL: // 加速度计校准值
            mutex_lock(&icm20608->lock);
            ret = icm20608_sensor_set(icm20608, ICM20_XA_OFFSET_H, chan->channel2, val);
            mutex_unlock(&icm20608->lock);
            break;
        default:
            ret = -EINVAL;
            break;
        }
        break;
    default:
        ret = -EINVAL;
        break;
    }
    return ret;
}
// 用户空间写入数据的格式，对数据进行变换后传递到内核空间
static int icm20608_write_raw_get_fmt(struct iio_dev *indio_dev, struct iio_chan_spec const *chan, long mask)
{
    printk("icm20608_write_raw_get_fmt\r\n");
    switch (mask)
    {
    case IIO_CHAN_INFO_SCALE:
        switch (chan->type)
        {
        case IIO_ANGL_VEL:
            return IIO_VAL_INT_PLUS_MICRO;
        default:
            return IIO_VAL_INT_PLUS_NANO;
        }
    default:
        return IIO_VAL_INT_PLUS_MICRO;
    }
    return -EINVAL;
}
static const struct iio_info icm20608_info = {
    .read_raw = icm20608_read_raw,
    .write_raw = icm20608_write_raw,
    .write_raw_get_fmt = &icm20608_write_raw_get_fmt,
};
int icm20608_driver_probe(struct spi_device *spi)
{
    int ret;
    struct ICM20608 *icm20608;
    struct iio_dev *indio_dev;
    // 申请iio_dev和icm20608内存
    indio_dev = devm_iio_device_alloc(&spi->dev, sizeof(*icm20608));
    if (indio_dev == NULL)
    {
        return -ENOMEM;
    }
    icm20608 = iio_priv(indio_dev);
    icm20608->spi = spi;
    spi_set_drvdata(spi, indio_dev);
    mutex_init(&icm20608->lock);
    // 初始化iio_dev
    indio_dev->dev.parent = &spi->dev;
    indio_dev->info = &icm20608_info;
    indio_dev->name = ICM20608_NAME;
    indio_dev->modes = INDIO_DIRECT_MODE; // 直接模式，提供sysfs接口
    indio_dev->channels = icm20608_channels;
    indio_dev->num_channels = ARRAY_SIZE(icm20608_channels);
    // 注册iio_dev
    ret = iio_device_register(indio_dev);
    if (ret < 0)
    {
        dev_err(&spi->dev, "iio_device_register failed\n");
        goto iio_register_failed;
    }
    // regmap申请与初始化
    icm20608->regmap_config.reg_bits = 8;
    icm20608->regmap_config.val_bits = 8;
    icm20608->regmap_config.read_flag_mask = 0x80;
    // 初始化spi总线的regmap
    icm20608->regmap = regmap_init_spi(spi, &icm20608->regmap_config);
    if (icm20608->regmap == NULL)
    {
        ret = PTR_ERR(icm20608->regmap);
        goto regmap_init_spi_failed;
    }
    spi->mode = SPI_MODE_0;
    spi_setup(spi);

    icm20608_reg_init(icm20608);
    printk("icm20608_driver_probe\n");
    return 0;
regmap_init_spi_failed:
    iio_device_unregister(indio_dev);
iio_register_failed:
    return ret;
}
int icm20608_driver_remove(struct spi_device *spi)
{
    struct iio_dev *indio_dev = spi_get_drvdata(spi);
    struct ICM20608 *icm20608 = iio_priv(indio_dev);
    regmap_exit(icm20608->regmap);
    iio_device_unregister(indio_dev);
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