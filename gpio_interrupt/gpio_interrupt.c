// pinctrl-bcm2711

/*
sudo apt update
sudo apt install raspberrypi-linux-headers

/sys/class/leds/<led-name>/
ls /sys/class/leds/

echo 1 | sudo tee /sys/class/leds/led0/brightness
echo 0 | sudo tee /sys/class/leds/led0/brightness
echo none | sudo tee /sys/class/leds/led0/trigger

while true; do
    echo 1 | sudo tee /sys/class/leds/led0/brightness
    sleep 1
    echo 0 | sudo tee /sys/class/leds/led0/brightness
    sleep 1
done
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio.h>          // legacy GPIO API
#include <linux/interrupt.h>

#define LED_PIN  17 + 512              // BCM 17 → use as Linux GPIO 17
#define BTN_PIN  27  + 512            // BCM 27

static int irq_num;
static bool led_state = false;

/* ------------------ Interrupt Handler ------------------ */
static irqreturn_t btn_isr(int irq, void *dev)
{
    led_state = !led_state;
    gpio_set_value(LED_PIN, led_state);

    pr_info("Button IRQ → LED = %d\n", led_state);
    return IRQ_HANDLED;
}

/* ------------------ Module Init ------------------ */
static int __init simple_irq_init(void)
{
    int ret;

    pr_info("Loading simple RPi4 GPIO IRQ module\n");

    /* --- Request LED GPIO --- */
    ret = gpio_request(LED_PIN, "rpi4_led");
    if (ret) {
        pr_err("Failed to request LED GPIO\n");
        return ret;
    }

    ret = gpio_direction_output(LED_PIN, 0);
    if (ret) {
        pr_err("Failed to set LED direction\n");
        goto err_led;
    }

    /* --- Request Button GPIO --- */
    ret = gpio_request(BTN_PIN, "rpi4_button");
    if (ret) {
        pr_err("Failed to request button GPIO\n");
        goto err_led;
    }

    ret = gpio_direction_input(BTN_PIN);
    if (ret) {
        pr_err("Failed to set button direction\n");
        goto err_btn;
    }

    /* --- Convert GPIO → IRQ number --- */
    irq_num = gpio_to_irq(BTN_PIN);
    if (irq_num < 0) {
        pr_err("Failed to map GPIO to IRQ\n");
        ret = irq_num;
        goto err_btn;
    }

    pr_info("BTN GPIO %d mapped to IRQ %d\n", BTN_PIN, irq_num);

    /* --- Request interrupt --- */
    ret = request_irq(irq_num,
                      btn_isr,
                      IRQF_TRIGGER_FALLING,
                      "rpi4_button_irq",
                      NULL);

    if (ret) {
        pr_err("Failed to request interrupt\n");
        goto err_btn;
    }

    pr_info("GPIO IRQ module loaded OK\n");
    return 0;

err_btn:
    gpio_free(BTN_PIN);
err_led:
    gpio_free(LED_PIN);
    return ret;
}

/* ------------------ Module Exit ------------------ */
static void __exit simple_irq_exit(void)
{
    free_irq(irq_num, NULL);

    gpio_set_value(LED_PIN, 0);
    gpio_free(BTN_PIN);
    gpio_free(LED_PIN);

    pr_info("GPIO IRQ module unloaded\n");
}

module_init(simple_irq_init);
module_exit(simple_irq_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("TechDhaba");
MODULE_DESCRIPTION("Simple RPi4 GPIO Interrupt ");
