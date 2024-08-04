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
#include <linux/input/mt.h>
#include <linux/input/touchscreen.h>
#include <asm/unaligned.h>
#define GT_CTRL_REG 0X8040     // GT9147控制寄存器
#define GT_MODSW_REG 0X804D    // GT9147模式切换寄存器
#define GT_9xx_CFGS_REG 0X8047 // GT9147配置起始地址寄存器
#define GT_1xx_CFGS_REG 0X8050 // GT9147配置起始地址寄存器
#define GT_CHECK_REG 0X80FF    // GT9147校验和寄存器
#define GT_PID_REG 0X8140      // GT9147产品ID寄存器

#define GT_GSTID_REG 0X814E  // GT9147当前检测到的触摸情况
#define GT_TP1_REG 0X814F    // 第一个触摸点数据地址
#define GT_TP2_REG 0X8157    // 第二个触摸点数据地址
#define GT_TP3_REG 0X815F    // 第三个触摸点数据地址
#define GT_TP4_REG 0X8167    // 第四个触摸点数据地址
#define GT_TP5_REG 0X816F    // 第五个触摸点数据地址
#define MAX_SUPPORT_POINTS 5 // 最多5点电容触摸

const u8 irq_table[] = {IRQ_TYPE_EDGE_RISING, IRQ_TYPE_EDGE_FALLING, IRQ_TYPE_LEVEL_LOW, IRQ_TYPE_LEVEL_HIGH}; // 触发方式
struct GT9147
{
    struct device_node *pnode; // 设备节点
    int irq_pin;               // 中断引脚
    int reset_pin;             // 复位引脚
    int irqnum;                // 中断号
    int irqtype;               // 中断类型
    int max_x;                 // 最大横坐标
    int max_y;                 // 最大纵坐标
    void *private_data;
    struct input_dev *input;   // input子系统
    struct i2c_client *client; // i2c子系统
};
static struct GT9147 gt9147;

// 从gt9147读出多个寄存器的数据
static int gt9147_read_regs(struct GT9147 *gt9147, u16 reg, void *buf, u8 length)
{
    int ret;
    u8 regdata[2];
    struct i2c_client *client = gt9147->client;
    struct i2c_msg msg[2];
    regdata[0] = reg >> 8;
    regdata[1] = reg & 0xFF;
    // 偏移读写位置
    msg[0].addr = client->addr; // 从机地址
    msg[0].flags = 0;           // 写
    msg[0].buf = regdata;       // 要写入的地址即寄存器地址
    msg[0].len = 2;             // 写入长度
    //  连续读数据
    msg[1].addr = client->addr; // 从机地址
    msg[1].flags = I2C_M_RD;    // 读
    msg[1].buf = buf;           // 读出的数据
    msg[1].len = length;        // 读出长度

    ret = i2c_transfer(client->adapter, msg, 2);
    if (ret == 2)
    {
        ret = 0;
    }
    else
    {
        printk("i2c rd failed=%d reg=%06x len=%d\n", ret, reg, length);
        ret = -EREMOTEIO;
    }
    return ret;
}

// 向gt9147写入多个寄存器的数据
static int gt9147_write_regs(struct GT9147 *gt9147, u16 reg, const void *buf, u8 length)
{
    int ret;
    u8 send_buf[256];
    struct i2c_client *client = gt9147->client;
    // 构建发送数据 寄存器首地址和要写入的数据
    struct i2c_msg msg;
    send_buf[0] = reg >> 8;   // 寄存器地址低8位
    send_buf[1] = reg & 0XFF; // 寄存器地址高8位
    memcpy(&send_buf[2], buf, length);
    // 偏移读写位置+连续写数据
    msg.addr = client->addr; // 从机地址
    msg.flags = 0;           // 写
    msg.buf = send_buf;      // 要写入的地址和数据
    msg.len = length + 2;    // 写入长度
    ret = i2c_transfer(client->adapter, &msg, 1);
    return ret;
}
static irqreturn_t gt9147_irq_handler(int irq, void *dev_id)
{

    int touch_num;
    int input_x, input_y;
    int id=0, ret;
    u8 data, touch_data[5];
    struct GT9147 *gt9147 = (struct GT9147 *)dev_id;
    ret = gt9147_read_regs(gt9147, GT_GSTID_REG, &data, 1);
    if (data == 0) // 没有触摸数据
    {
        return IRQ_HANDLED;
    }
    else
    { // 统计触摸点数据
        touch_num = data & 0x0f;
    }
    if (touch_num)
    {
        gt9147_read_regs(gt9147, GT_TP1_REG, touch_data, 5);
        id = touch_data[0] & 0x0f;
        if (id == 0)
        {
            input_x = touch_data[1] | (touch_data[2] << 8);
            input_y = touch_data[3] | (touch_data[4] << 8);
            input_mt_slot(gt9147->input, id);
            input_mt_report_slot_state(gt9147->input, MT_TOOL_FINGER, true);
            input_report_abs(gt9147->input, ABS_MT_POSITION_X, input_x);
            input_report_abs(gt9147->input, ABS_MT_POSITION_Y, input_y);
        }
    }
    else if (touch_num == 0) // 单点触摸释放
    {
        input_mt_slot(gt9147->input, id);
        input_mt_report_slot_state(gt9147->input, MT_TOOL_FINGER, false);
    }
    input_mt_report_pointer_emulation(gt9147->input, true);
    input_sync(gt9147->input);
    data = 0x00; // 向0X814E寄存器写0
    gt9147_write_regs(gt9147, GT_GSTID_REG, &data, 1);
    return IRQ_HANDLED;
}
static int gt9147_ts_reset(struct i2c_client *client, struct GT9147 *gt9147)
{
    int ret;
    // 申请复位IO
    if (gpio_is_valid(gt9147->reset_pin))
    {
        ret = devm_gpio_request_one(&client->dev, gt9147->reset_pin, GPIOF_OUT_INIT_HIGH, "gt9147 reset");
        if (ret < 0)
        {
            return ret;
        }
        msleep(5);
        gpio_set_value(gt9147->reset_pin, 1); // 输出高电平，停止复位
        msleep(300);
    }
    // 申请中断IO
    if (gpio_is_valid(gt9147->irq_pin))
    {
        ret = devm_gpio_request_one(&client->dev, gt9147->irq_pin, GPIOF_OUT_INIT_HIGH, "gt9147 irq");
        if (ret < 0)
        {
            dev_err(&client->dev, "Failed to request GPIO %d, error %d\n", gt9147->irq_pin, ret);
            return ret;
        }
    }
    // 严格按照GT9147时序要求初始化GT9147
    gpio_set_value(gt9147->reset_pin, 0); // 复位GT9147
    msleep(10);
    gpio_set_value(gt9147->reset_pin, 1); // 停止复位GT9147
    msleep(10);
    gpio_set_value(gt9147->irq_pin, 0); // 拉低INT引脚
    msleep(50);
    gpio_direction_input(gt9147->irq_pin); // INT引脚设置为输入
    return 0;
}
static int gt9147_ts_irq(struct i2c_client *client, struct GT9147 *gt9147)
{
    int ret;
    // 申请中断资源
    ret = devm_request_threaded_irq(&client->dev, client->irq, NULL,
                                    gt9147_irq_handler, irq_table[gt9147->irqtype] | IRQF_ONESHOT,
                                    client->name, gt9147);
    if (ret < 0)
    {
        dev_err(&client->dev, "Unable to request touchscreen IRQ.\n");
        return ret;
    }
    return 0;
}
static int gt9147_read_firmware(struct i2c_client *client, struct GT9147 *gt9147)
{
    int ret = 0, version = 0;
    u16 id = 0;
    u8 data[7] = {0};
    char id_str[5];
    ret = gt9147_read_regs(gt9147, GT_PID_REG, data, 6);
    if (ret)
    {
        dev_err(&client->dev, "Unable to read PID.\n");
        return ret;
    }
    memcpy(id_str, data, 4);
    id_str[4] = 0;
    if (kstrtou16(id_str, 10, &id))
        id = 0x1001;
    version = get_unaligned_le16(&data[4]);
    dev_info(&client->dev, "ID %d, version: %04x\n", id, version);
    switch (id)
    { /* 由于不同的芯片配置寄存器地址不一样需要判断一下  */
    case 1151:
    case 1158:
    case 5663:
    case 5688: /* 读取固件里面的配置信息  */
        ret = gt9147_read_regs(gt9147, GT_1xx_CFGS_REG, data, 7);
        break;
    default:
        ret = gt9147_read_regs(gt9147, GT_9xx_CFGS_REG, data, 7);
        break;
    }
    if (ret)
    {
        dev_err(&client->dev, "Unable to read Firmware.\n");
        return ret;
    }
    gt9147->max_x = (data[2] << 8) + data[1];
    gt9147->max_y = (data[4] << 8) + data[3];
    gt9147->irqtype = data[6] & 0x3;
    printk("X_MAX: %d, Y_MAX: %d, TRIGGER: 0x%02x", gt9147->max_x, gt9147->max_y, gt9147->irqtype);

    return 0;
}
int gt9147_driver_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
    u8 data;
    int ret;
    gt9147.client = client;
    // 获取设备树中gt9147设备的中断和复位引脚
    gt9147.irq_pin = of_get_named_gpio(client->dev.of_node, "interrupt-gpios", 0);
    gt9147.reset_pin = of_get_named_gpio(client->dev.of_node, "reset-gpios", 0);
    // 复位gt9147
    ret = gt9147_ts_reset(client, &gt9147);
    if (ret < 0)
    {
        printk("gt9147_driver_probe: gt9147_ts_reset failed!\n");
        return ret;
    }
    // 初始化gt9147
    data = 0x02;
    gt9147_write_regs(&gt9147, GT_CTRL_REG, &data, 1); // 软复位
    mdelay(100);
    data = 0x0;
    gt9147_write_regs(&gt9147, GT_CTRL_REG, &data, 1); // 停止软复位
    mdelay(100);
    // 初始化gt9147，读取固件信息
    ret = gt9147_read_firmware(client, &gt9147);
    if (ret != 0)
    {
        printk("gt9147_driver_probe: gt9147_read_firmware check failed!\n");
        return ret;
    }

    // 申请input设备
    gt9147.input = devm_input_allocate_device(&client->dev);
    if (!gt9147.input)
    {
        printk("gt9147_driver_probe: devm_input_allocate_device failed!\n");
        return -ENOMEM;
    }
    gt9147.input->name = client->name;
    gt9147.input->id.bustype = BUS_I2C;
    gt9147.input->dev.parent = &client->dev;

    __set_bit(EV_KEY, gt9147.input->evbit);
    __set_bit(EV_ABS, gt9147.input->evbit);
    __set_bit(BTN_TOUCH, gt9147.input->keybit);

    input_set_abs_params(gt9147.input, ABS_X, 0, gt9147.max_x, 0, 0);
    input_set_abs_params(gt9147.input, ABS_Y, 0, gt9147.max_y, 0, 0);
    input_set_abs_params(gt9147.input, ABS_MT_POSITION_X, 0, gt9147.max_x, 0, 0);
    input_set_abs_params(gt9147.input, ABS_MT_POSITION_Y, 0, gt9147.max_y, 0, 0);
    ret = input_mt_init_slots(gt9147.input, MAX_SUPPORT_POINTS, 0);
    if (ret < 0)
    {
        printk("gt9147_driver_probe: input_mt_init_slots failed!\n");
        return ret;
    }
    // 最后初始化中断
    ret = gt9147_ts_irq(client, &gt9147);
    if (ret < 0)
    {
        printk("gt9147_driver_probe: gt9147_ts_irq failed!\n");
        return ret;
    }
    ret = input_register_device(gt9147.input);
    if (ret < 0)
    {
        printk("gt9147_driver_probe: input_register_device failed!\n");
        return ret;
    }
    printk("gt9147_driver_probe\n");
    return 0;
}
int gt9147_driver_remove(struct i2c_client *client)
{
    input_unregister_device(gt9147.input);
    printk("gt9147_driver_remove\n");
    return 0;
}
struct of_device_id gt9147_of_match[] = {
    {.compatible = "goodix,gt9147"},
    {},
};
struct i2c_device_id gt9147_id[] = {
    {"goodix,gt9147", 0},
    {},
};
static struct i2c_driver gt9147_driver = {
    .driver = {
        .name = "gt9147-liu",
        .owner = THIS_MODULE,
        .of_match_table = gt9147_of_match,
    },
    .id_table = gt9147_id,
    .probe = gt9147_driver_probe,
    .remove = gt9147_driver_remove,
};

static int __init I2C_GT9147_init(void)
{
    int ret;
    printk("I2C_GT9147_init\n");
    ret = i2c_add_driver(&gt9147_driver);
    return ret;
}

static void __exit I2C_GT9147_exit(void)
{
    i2c_del_driver(&gt9147_driver);
    printk("I2C_GT9147_exit\n");
}

module_init(I2C_GT9147_init);
module_exit(I2C_GT9147_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("liu");