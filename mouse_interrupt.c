#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <asm/io.h>

#define MOUSE_IRQ 12
#define MOUSE_DATA_PORT 0x60   
#define MOUSE_STATUS_PORT 0x64 

static irqreturn_t mouse_irq_handler(int irq, void *dev_id)
{
    unsigned char status;
    unsigned char data;

    status = inb(MOUSE_STATUS_PORT);

    if (status & 0x20)
    {
        data = inb(MOUSE_DATA_PORT);
        printk(KERN_INFO "Mouse IRQ: Data byte = 0x%02X\n", data);
    }

    return IRQ_HANDLED;
}

static int __init mouse_irq_init(void)
{
    int res;

    printk(KERN_INFO "Loading custom mouse IRQ handler...\n");

    res = request_irq(MOUSE_IRQ,
                      mouse_irq_handler,
                      IRQF_SHARED,
                      "mouse_irq_handler",
                      (void *)mouse_irq_handler);

    if (res)
    {
        printk(KERN_ERR "mouse_irq: Cannot register IRQ %d\n", MOUSE_IRQ);
        return res;
    }

    printk(KERN_INFO "mouse_irq: IRQ handler registered successfully.\n");
    return 0;
}

static void __exit mouse_irq_exit(void)
{
    free_irq(MOUSE_IRQ, (void *)mouse_irq_handler);
    printk(KERN_INFO "mouse_irq: IRQ handler removed.\n");
}

module_init(mouse_irq_init);
module_exit(mouse_irq_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("YAWAR");
MODULE_DESCRIPTION("Very basic mouse interrupt handler.");