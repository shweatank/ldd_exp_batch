#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/init.h>
#include<linux/gpio.h>
#include<linux/interrupt.h>

#define DEVICE_N "gpio_interrupt"
#define LED 512+17
#define button 512+27

static int irq_num;
static bool led_state;
static int MAJOR;

static irqreturn_t btn_isr(int irq, void *dev){
	led_state = !led_state;
	gpio_set_value(LED, led_state);
	return IRQ_HANDLED;
}


static int __init start(void){
	MAJOR= register_chrdev(0, DEVICE_N, &fops);
	if(MAJOR<0){
		pr_info("device not register\n");
		return 0;
	}
	pr_info("device register\n");
	int ret;
	ret=gpio_request(LED, "led_pin");
	if(ret){
		pr_info("failed led request\n");
		return ret;
	}

	ret = gpio_direction_output(LED, 0);
	if(ret){
		pr_info("failed to set led direction\n");
		goto ere_led;
	}

	ret = gpio_request(button, "led_pin");
	if(ret){
		pr_info("failed button request\n");
		goto err_btn;
	}
	ret = gpio_direction_output(button, 0);
	if(ret){
		pr_info("failed to set button direction\n");
		goto err_btn;
	}
	
	irq_num = gpio_to_irq(button);
	if(irq_num < 0){
		pr_err("failed to map gpio to irq\n");
		ret = irq_num;
		goto err_btn;
	}

	pr_info("btn gpio %d map IRQ %d", button, irq_num);


	ret = request_irq(irq_num, btn_isr, IRQ_TRIGGER_FAILLING, "irq_request", NULL);

	if(ret){
		pr_err("failed to request interrupt\n");
		goto err_btn;
	}
	pr_info("gpio irq module loaded ok\n");
	return 0;

err_btn:
	gpio_free(button);
err_led:
	gpio_free(LED);
}

static void __exit end(void){
	free_irq(irq_num, NULL);
	gpio_free(button);
	gpio_free(LED);
	unregister_chrdev(MAJOR, DEVICE_N);
	pr_info("device unregister\n");
}

module_init(start);
module_exit(exit);
MODULE_LICENSE("GPL");
