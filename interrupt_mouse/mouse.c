#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <asm/io.h>

#define MOUSE_IRQ 12
#define MOUSE_DATA_PORT 0x60

static int dev_id;

static irqreturn_t mouse_irq_handler(int irq, void *dev_id_ptr)
{
    unsigned char data = inb(MOUSE_DATA_PORT);
    printk(KERN_INFO "Mouse IRQ fired: data = 0x%02X\n", data);
    return IRQ_HANDLED;
}

static int __init mouse_irq_init(void)
{
    int result;

    printk(KERN_INFO "Loading mouse IRQ handler...\n");

    result = request_irq(MOUSE_IRQ, mouse_irq_handler,
                         IRQF_SHARED, "mouse_irq", &dev_id);

    if (result) {
        printk(KERN_ERR "ERROR: cannot register IRQ 12\n");
        return result;
    }

    printk(KERN_INFO "Mouse IRQ handler registered.\n");
    return 0;
}

static void __exit mouse_irq_exit(void)
{
    free_irq(MOUSE_IRQ, &dev_id);
    printk(KERN_INFO "Mouse IRQ handler removed.\n");
}

module_init(mouse_irq_init);
module_exit(mouse_irq_exit);

MODULE_LICENSE("GPL");

