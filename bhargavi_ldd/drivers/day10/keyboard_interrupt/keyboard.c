#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <asm/io.h>

#define KEYBOARD_IRQ 1
#define KEYBOARD_PORT 0x60

static int dev_id;

irqreturn_t keyboard_handler(int irq, void *dev)
{
    unsigned char scancode;
    scancode = inb(KEYBOARD_PORT);
    printk(KERN_INFO "Key pressed: %x\n", scancode);
    return IRQ_HANDLED;
}

static int __init keyboard_init(void)
{
    int ret;
    ret = request_irq(KEYBOARD_IRQ, keyboard_handler,
                      IRQF_SHARED, "keyboard_driver", &dev_id);

    if (ret) {
        printk(KERN_ERR "request_irq failed: %d\n", ret);
        return ret;
    }

    printk(KERN_INFO "Keyboard interrupt loaded\n");
    return 0;
}

static void __exit keyboard_exit(void)
{
    free_irq(KEYBOARD_IRQ, &dev_id);
    printk(KERN_INFO "Keyboard interrupt unloaded\n");
}

MODULE_LICENSE("GPL");
module_init(keyboard_init);
module_exit(keyboard_exit);

