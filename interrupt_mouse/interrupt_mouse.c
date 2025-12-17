#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/interrupt.h>
#include<asm/io.h>

#define MOUSE_IRQ 12
#define MOUSE_DATA_PORT 0x60

static unsigned char packet[3];

static irqreturn_t mouse_irq_handler(int irq, void *dev_id){
	unsigned char scancode = inb(MOUSE_DATA_PORT);
	printk(KERN_INFO "keyboard IRQ: scancode = 0x%02x\n", scancode);
	return IRQ_HANDLED;

}


static int __init mouse_irq_init(void)
{
    int result;

    printk(KERN_INFO "Loading custom keyboard IRQ handler\n");

    result = request_irq(MOUSE_IRQ, mouse_irq_handler, IRQF_SHARED, "mouse_irq", (void *) mouse_irq_handler);

    if (result) {
        printk(KERN_ERR "mouse_irq: cannot register IRQ %d\n",MOUSE_IRQ);
        return result;
    }   

    printk(KERN_INFO "mouse_irq: handler registered successfully\n");
    return 0;
}

static void __exit mouse_irq_exit(void)
{
    free_irq(MOUSE_IRQ, (void *)mouse_irq_handler);
    printk(KERN_INFO "Mouse_irq: handler removed\n");
}



module_init(mouse_irq_init);
module_exit(mouse_irq_exit);

MODULE_LICENSE("GPL");

