#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/module.h>
#include<linux/interrupt.h>
#include<linux/io.h>

#define KEYBOARD_IRQ 1
#define KBD_DATA_PORT 0x60

irqreturn_t keyboard_irq_handler(int irq,void *dev_id)
{
	unsigned char scancode;

	scancode = inb(KBD_DATA_PORT);

	printk("interrupt is occured from the keyboard\n");

	return IRQ_HANDLED;
}

static int __init hi(void)
{
	int result;

	printk(KERN_INFO "Loading custom keyboard IRQ  Handler...\n");

	result=request_irq(KEYBOARD_IRQ, keyboard_irq_handler,IRQF_SHARED,"keyboardinterrupt",(void *)(keyboard_irq_handler));
	if(result)
		printk(KERN_ERR "error occures while resgistering the data\n");
	else
		printk(KERN_INFO "working fine\n");
	return 0;
}

static void __exit hello(void)
{
	free_irq(KEYBOARD_IRQ,(void *)(keyboard_irq_handler));
	pr_info("exit function is invoked\n");
}

module_init(hi);
module_exit(hello);

MODULE_LICENSE("GPL");
