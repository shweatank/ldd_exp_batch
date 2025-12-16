#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio/consumer.h>
#include <linux/interrupt.h>
#include <linux/timer.h>
#include <linux/err.h>
#include <linux/of.h>
#include <linux/gpio.h>
#include <linux/gpio/driver.h>
 
#define LED_PIN   17 + 512     // BCM GPIO17 → LED
#define BTN_PIN   27  + 512   // BCM GPIO27 → Input with interrupt
 
static struct gpio_desc *led;
static struct gpio_desc *btn;
 
static int irq_num;
static bool led_state = false;
 
/* ---------- Interrupt Handler ---------- */
static irqreturn_t btn_isr(int irq, void *dev_id)
{
    led_state = !led_state;
    gpiod_set_value(led, led_state);   // Toggle LED
    pr_info("IRQ: Button pressed → LED toggled = %d\n", led_state);
    return IRQ_HANDLED;
}
 
/* ---------- Module Init ---------- */
static int __init irq_blink_init(void)
{
    struct gpio_chip *chip;
 
    pr_info("Loading RPi4 GPIO IRQ + LED module\n");
 
    /* Find chip: pinctrl-bcm2711 */
    chip = gpiochip_find("pinctrl-bcm2711", (void *)gpiochip_find);
    if (!chip) {
        pr_err("Could not find BCM2711 GPIO chip\n");
        return -ENODEV;
    }
 
    /* LED GPIO descriptor */
    led = gpiochip_get_desc(chip, LED_PIN);
    if (IS_ERR(led)) {
        pr_err("Failed to get LED GPIO %d\n", LED_PIN);
        return PTR_ERR(led);
    }
 
    if (gpiod_direction_output(led, 0)) {
        pr_err("Failed to configure LED output\n");
        return -EINVAL;
    }
 
    /* Button GPIO descriptor */
    btn = gpiochip_get_desc(chip, BTN_PIN);
    if (IS_ERR(btn)) {
        pr_err("Failed to get BTN GPIO %d\n", BTN_PIN);
        return PTR_ERR(btn);
    }
 
    if (gpiod_direction_input(btn)) {
        pr_err("Failed to configure BTN as input\n");
        return -EINVAL;
    }
 
    /* Convert GPIO desc → Linux IRQ */
    irq_num = gpiod_to_irq(btn);
    if (irq_num < 0) {
        pr_err("Failed to map GPIO to IRQ\n");
        return irq_num;
    }
 
    pr_info("Button GPIO %d mapped to IRQ %d\n", BTN_PIN, irq_num);
 
    /* Request interrupt */
    if (request_irq(irq_num, btn_isr,
                    IRQF_TRIGGER_FALLING | IRQF_ONESHOT,
                    "rpi4_button_irq", NULL)) {
        pr_err("Failed to request IRQ\n");
        return -EIO;
    }
 
    pr_info("IRQ handler registered successfully\n");
    return 0;
}
 
/* ---------- Module Exit ---------- */
static void __exit irq_blink_exit(void)
{
    free_irq(irq_num, NULL);
    if (led) gpiod_put(led);
    if (btn) gpiod_put(btn);
 
    pr_info("GPIO IRQ + LED module unloaded\n");
}
 
module_init(irq_blink_init);
module_exit(irq_blink_exit);
 
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Raspberry Pi 4 GPIO Interrupt + LED Blink");
MODULE_AUTHOR("TechDhaba");
 
