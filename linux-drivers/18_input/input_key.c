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

#define INPUT_KEY_DEV_NAME "INPUT_KEY_DEV_NAME"
#define INPUT_KEY_DEV_COUNT 1
#define KEY0_PUSH_VALUE 0xff
#define KEY0_RELEASE_VALUE 0x00
#define KEY_PUSH_STATE 0x10
#define KEY_RELEASE_STATE 0x20
struct IRQ_INFO_KEY
{
    int gpio_num;                        // gpio编号
    int irq_num;                         // 中断号
    unsigned char value;                 // 按键值
    char name[15];                       // 中断名字
    irqreturn_t (*handler)(int, void *); // 中断处理函数
};
// INPUT_KEY_DEV设备结构体
struct INPUT_KEY_DEV
{
    struct device_node *pdevnode; // 设备节点
    int gpio_num;
    unsigned char current_key_num;
    struct IRQ_INFO_KEY irq_keys[INPUT_KEY_DEV_COUNT];
    struct input_dev *input_dev;
    struct timer_list timer0;
};
static struct INPUT_KEY_DEV input_key_dev;

// 中断处理函数
irqreturn_t key0_handler(int irq_num, void *parg)
{
    struct INPUT_KEY_DEV *pinput_key_dev = (struct INPUT_KEY_DEV *)parg;
    pinput_key_dev->current_key_num = 0;
    mod_timer(&pinput_key_dev->timer0, jiffies + msecs_to_jiffies(15));
    return IRQ_RETVAL(IRQ_HANDLED);
}
static void timer_func(unsigned long parg)
{
    int value;
    struct INPUT_KEY_DEV *pinput_key_dev = (struct INPUT_KEY_DEV *)parg;
    value = gpio_get_value(pinput_key_dev->irq_keys[pinput_key_dev->current_key_num].gpio_num);
    if (value == 0) // 按下动作消抖结束-->按键按下
    {
        // 上报按键值  KEY_XXX一般专用于键盘按键  其他各种按键可以使用button
        input_event(pinput_key_dev->input_dev, EV_KEY, BTN_0, 1);
        input_sync(pinput_key_dev->input_dev);
    }
    else // 松开动作消抖结束-->按键松开
    {
        // 上报按键值
        input_event(pinput_key_dev->input_dev, EV_KEY, BTN_0, 0);
        input_sync(pinput_key_dev->input_dev);
    }
}

static int KEY0_init(void)
{
    int ret, i, gpio_count, irq_count;
    // 获取设备节点
    input_key_dev.pdevnode = of_find_node_by_path("/key0");
    if (input_key_dev.pdevnode == NULL)
    {
        printk("INPUT_KEY_DEV_init: of_find_node_by_path failed!\n");
        ret = -EINVAL;
        goto failed;
    }
    // 通过设备树中该设备节点的gpios属性获取gpio编号
    for (i = 0; i < INPUT_KEY_DEV_COUNT; i++)
    {
        input_key_dev.irq_keys[i].gpio_num = of_get_named_gpio(input_key_dev.pdevnode, "key0-gpios", i);
        if (input_key_dev.irq_keys[i].gpio_num < 0)
        {
            printk("INPUT_KEY_DEV_init: of_get_named_gpio failed!\n");
            ret = -EINVAL;
            goto failed;
        }
    }

    // 初始化key所使用的gpio，并设置成中断模式
    for (i = 0; i < INPUT_KEY_DEV_COUNT; i++)
    {
        memset(input_key_dev.irq_keys[i].name, 0, sizeof(input_key_dev.irq_keys[i].name));
        sprintf(input_key_dev.irq_keys[i].name, "key%d", i);
        ret = gpio_request(input_key_dev.irq_keys[i].gpio_num, input_key_dev.irq_keys[i].name);
        if (ret < 0)
        {
            printk("INPUT_KEY_DEV_init: gpio_request failed!\n");
            ret = -EINVAL;
            gpio_count = i;
            goto gpio_failed;
        }

        // 将GPIO引脚设置为输出模式
        ret = gpio_direction_input(input_key_dev.irq_keys[i].gpio_num);
        if (ret < 0)
        {
            printk("INPUT_KEY_DEV_init: gpio_direction_output failed!\n");
            ret = -EINVAL;
            gpio_count = i + 1;
            goto gpio_failed;
        }
        input_key_dev.irq_keys[i].irq_num = irq_of_parse_and_map(input_key_dev.pdevnode, i);

#if 0
        input_key_dev.irq_keys[i].irq_num=gpio_to_irq(input_key_dev.irq_keys[i].gpio_num);
#endif
        if (input_key_dev.irq_keys[i].irq_num < 0)
        {
            printk("INPUT_KEY_DEV_init: irq_of_parse_and_map failed!\n");
            ret = -EINVAL;
            gpio_count = i + 1;
            goto gpio_failed;
        }
    }
    gpio_count = INPUT_KEY_DEV_COUNT;
    // 申请中断 只用到了key0
    input_key_dev.irq_keys[0].handler = key0_handler;
    for (i = 0; i < INPUT_KEY_DEV_COUNT; i++)
    {
        ret = request_irq(input_key_dev.irq_keys[i].irq_num, input_key_dev.irq_keys[i].handler, IRQF_TRIGGER_FALLING | IRQF_TRIGGER_RISING, input_key_dev.irq_keys[i].name, &input_key_dev);
        if (ret < 0)
        {
            printk("INPUT_KEY_DEV_init: request_irq failed!\n");
            ret = -EINVAL;
            irq_count = i;
            goto irq_failed;
        }
    }
    irq_count = INPUT_KEY_DEV_COUNT;

    return 0;

irq_failed:
    for (i = 0; i < irq_count; i++)
    {
        free_irq(input_key_dev.irq_keys[i].irq_num, &input_key_dev);
    }
gpio_failed:
    for (i = 0; i < gpio_count; i++)
    {
        gpio_free(input_key_dev.irq_keys[i].gpio_num);
    }
failed:
    return ret;
}
static int __init INPUT_KEY_DEV_init(void)
{
    int ret;
    ret = KEY0_init();
    if (ret < 0)
    {
        printk("INPUT_KEY_DEV_init: key_init failed!\n");
        goto failed;
    }

    init_timer(&input_key_dev.timer0);
    input_key_dev.timer0.function = timer_func;
    input_key_dev.timer0.data = (unsigned long)&input_key_dev;
    // 注册input_dev
    input_key_dev.input_dev = input_allocate_device();
    if (input_key_dev.input_dev == NULL)
    {
        ret = -EINVAL;
        goto failed;
    }
    // 初始化input_dev
    input_key_dev.input_dev->name = INPUT_KEY_DEV_NAME;
#if 0
    // 设置产生哪些事件
    __set_bit(EV_KEY, input_key_dev.input_dev->evbit); // 按键事件
    __set_bit(EV_REP, input_key_dev.input_dev->evbit); // 重复事件
    // 设置产生哪些按键
    __set_bit(BTN_0, input_key_dev.input_dev->keybit); // 按键编号
#endif
#if 0
    input_key_dev.input_dev->evbit[0] = BIT_MASK(EV_KEY) | BIT_MASK(EV_REP);
    input_key_dev.input_dev->keybit[BIT_WORD(BTN_0)] |= BIT_MASK(BTN_0);
#endif
    input_key_dev.input_dev->evbit[0] = BIT_MASK(EV_KEY) | BIT_MASK(EV_REP);
    input_set_capability(input_key_dev.input_dev, EV_KEY, BTN_0);
    ret = input_register_device(input_key_dev.input_dev);
    if (ret < 0)
    {
        goto fail_input_register;
    }
    return 0;
fail_input_register:
    input_free_device(input_key_dev.input_dev);
failed:
    return ret;
}
static void __exit INPUT_KEY_DEV_exit(void)
{
    int i;
    // 释放gpio号
    for (i = 0; i < INPUT_KEY_DEV_COUNT; i++)
    {
        free_irq(input_key_dev.irq_keys[i].irq_num, &input_key_dev);
        gpio_free(input_key_dev.irq_keys[i].gpio_num);
    }
    del_timer_sync(&input_key_dev.timer0);
    // 注销input_dev
    input_unregister_device(input_key_dev.input_dev);
    input_free_device(input_key_dev.input_dev);
    printk("INPUT_KEY_DEV_exit\n");
}
module_init(INPUT_KEY_DEV_init);
module_exit(INPUT_KEY_DEV_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("liu");