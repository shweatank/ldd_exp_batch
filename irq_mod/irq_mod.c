#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/interrupt.h>
#include<asm/io.h>

#define KEYBOARD_IRQ 1 //IRQ number for keyboard
#define KBD_DATA_PORT 0x60   //port to read keyboard scancode

//interrupt handler

irqreturn_t keyboard_irq_handler(int irq, void *dev_id){
	unsigned char scancode;
	//read scancode from port 0x60;
	scancode = inb(KBD_DATA_PORT);//inb is kernel function
	printk(KERN_INFO "keyboard IRQ: Scancode = 0x02X\n",scancode);
	return IRQ_HANDLED;
}

static int __init keyboard_irq_init(void){
	int result;
	printk(KERN_INFO "loading custom keyboard IRQ handler..\n");
	//request IRQ 1 (keyboard interrupt);
	result =request_irq(KEYBOARD_IRQ,keyboard_irq_handler,IRQF_SHARED," keyboard_irq_handler",(void*)(keyboard_irq_handler));
	if(result){
		printk(KERN_ERR "keyboard_irq:cannot register IRQ %d\n",KEYBOARD_IRQ);
		return result;
	}
	printk(KERN_INFO "keybioard_irq: IRQ handler register successfully:\n");
	return 0;
}
static void __exit keyboard_irq_exit(void){
	free_irq(KEYBOARD_IRQ,(void*)(keyboard_irq_handler));
	printk(KERN_INFO "keyboard_irq: IRQ handler removed\n");
}
module_init(keyboard_irq_init);
module_exit(keyboard_irq_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("swapnil");
MODULE_DESCRIPTION("keyboard IRQ Handler for x86 linux:\n");
