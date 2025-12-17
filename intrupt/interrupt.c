#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <asm/io.h>

#define KEYBOARD_IRQ 1
#define KBD_DATA_PORT 0x60

static int major;
static char buffer[BUF_SIZE];
static char result[BUF_SIZE];
static int result_size = 0;

static int my_open(struct inode *inode, struct file *file){
	pr_info("device open\n");
	return 0;
 int my_release(struct inode *inode, struct file *file){
	pr_info("device close\n");
	return 0;
}

static irqreturn_t keyboard_irq_handler(int irq, void *dev_id)
{
    unsigned char scancode;

    scancode = inb(KBD_DATA_PORT);
    printk(KERN_INFO "keyboard IRQ: scancode = 0x%02x\n", scancode);
    if(scancode == 30){
    	printk(KERN_INFO "this key is : A");
    }

    return IRQ_HANDLED;
}

static int __init keyboard_irq_init(void)
{
    int result;

    printk(KERN_INFO "Loading custom keyboard IRQ handler\n");

    result = request_irq(KEYBOARD_IRQ, keyboard_irq_handler, IRQF_SHARED, "keyboard_irq_handler", (void *) keyboard_irq_handler);

    if (result) {
        printk(KERN_ERR "keyboard_irq: cannot register IRQ %d\n", KEYBOARD_IRQ);
        return result;
    }

    printk(KERN_INFO "keyboard_irq: handler registered successfully\n");
    return 0;
}

static void __exit keyboard_irq_exit(void)
{
    free_irq(KEYBOARD_IRQ, (void *) keyboard_irq_handler);
    printk(KERN_INFO "keyboard_irq: handler removed\n");
}



module_init(keyboard_irq_init);
module_exit(keyboard_irq_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Keyboard IRQ handler for x86 Linux");
MODULE_AUTHOR("Your Name");

