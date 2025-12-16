#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/jiffies.h>

#define IR_GPIO_PIN  529   // Change according to your board

static int ir_irq;
static unsigned long last_detect_time;

/* Interrupt handler */
static irqreturn_t ir_irq_handler(int irq, void *dev_id)
{
    last_detect_time = jiffies;

    printk(KERN_INFO "IR SENSOR: Object detected (GPIO LOW)\n");

    return IRQ_HANDLED;
}

static int __init ir_driver_init(void)
{
    int ret;

    printk(KERN_INFO "IR SENSOR DRIVER INIT\n");

    /* Request GPIO */
    ret = gpio_request(IR_GPIO_PIN, "ir_sensor_gpio");
    if (ret) {
        printk(KERN_ERR "Failed to request GPIO\n");
        return ret;
    }

    /* Set GPIO as input */
    gpio_direction_input(IR_GPIO_PIN);

    /* Convert GPIO to IRQ */
    ir_irq = gpio_to_irq(IR_GPIO_PIN);
    if (ir_irq < 0) {
        printk(KERN_ERR "Failed to get IRQ\n");
        gpio_free(IR_GPIO_PIN);
        return ir_irq;
    }

    /* Request IRQ on falling edge */
    ret = request_irq(ir_irq,
                      ir_irq_handler,
                      IRQF_TRIGGER_FALLING,
                      "ir_sensor_irq",
                      NULL);

    if (ret) {
        printk(KERN_ERR "Failed to request IRQ\n");
        gpio_free(IR_GPIO_PIN);
        return ret;
    }

    printk(KERN_INFO "IR sensor interrupt registered on GPIO %d\n", IR_GPIO_PIN);
    return 0;
}

static void __exit ir_driver_exit(void)
{
    free_irq(ir_irq, NULL);
    gpio_free(IR_GPIO_PIN);
    printk(KERN_INFO "IR SENSOR DRIVER EXIT\n");
}

module_init(ir_driver_init);
module_exit(ir_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("You");
MODULE_DESCRIPTION("IR Sensor GPIO Interrupt Driver");

