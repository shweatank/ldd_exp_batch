#include<linux/kernel.h>
#include<linux/gpio.h>
#include<linux/timer.h>
#include<linux/module.h>
#include<linux/init.h>

#define LED_PIN_1 529
#define LED_PIN_2 539
static int led_1=0;
static int led_2=0;
static struct timer_list led1_timer;
static struct timer_list led2_timer;
static void led_blink_1(struct timer_list *tm){
	led_1=!led_1;
	gpio_set_value(LED_PIN_1, led_1);
	printk(KERN_INFO "Led1 = %d\n", led_1);
	mod_timer(&led1_timer, jiffies+msecs_to_jiffies(1000));
}

static void led_blink_2(struct timer_list *tm){
	led_2=!led_2;
	gpio_set_value(LED_PIN_2, led_2);
	printk(KERN_INFO "Led2 = %d\n", led_2);
	mode_timer(&led2_timer, jiffies+msecs_to_jiffies(1000));
}

static int __init basic_init(void){
	if(gpio_is_valid(LED_PIN_1) == false){
		printk(KERNEL_INFO "the first gpio pin is not valid\n");
		return -1;
	}

	if(gpio_is_valid(LED_PIN_2) == false){
		printk(KERNEL_INFO)
	}

	ret = gpio_request(LED)
}
