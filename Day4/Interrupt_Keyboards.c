#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/io.h>

#define KEYBOARD_IRQ 1
#define KBD_DATA_PORT 0x60

/* Hardcoded values */
static int val1 = 20;
static int val2 = 5;

/* Scancodes for 'a' and 's' keys (make codes, not break codes) */
#define SCANCODE_A 0x1E
#define SCANCODE_S 0x1F
#define SCANCODE_M 0x20
#define SCANCODE_M 0x32
irqreturn_t keyboard_irq_handler(int irq, void *dev_id)
{
    unsigned char scancode;

    scancode = inb(KBD_DATA_PORT);

    /* Print interrupt info */
    printk(KERN_INFO "Keyboard IRQ triggered: scancode = 0x%x\n", scancode);

    /* Check for 'a' key press (make code only) */
    if (scancode == SCANCODE_A) {
        int result = val1 + val2;
        printk(KERN_INFO "'a' pressed → Adding %d + %d = %d\n", val1, val2, result);
    }

    /* Check for 's' key press */
    else if (scancode == SCANCODE_S) {
        int result = val1 - val2;
        printk(KERN_INFO "'s' pressed → Subtracting %d - %d = %d\n", val1, val2, result);
    }

    return IRQ_HANDLED;
}

static int __init hi(void)
{
    int result;

    printk(KERN_INFO "Loading custom keyboard IRQ Handler...\n");

    result = request_irq(KEYBOARD_IRQ, keyboard_irq_handler,
                         IRQF_SHARED, "keyboardinterrupt",
                         (void *)(keyboard_irq_handler));
    if (result)
        printk(KERN_ERR "Error registering IRQ handler\n");
    else
        printk(KERN_INFO "Keyboard IRQ handler installed\n");

    return 0;
}

static void __exit hello(void)
{
    free_irq(KEYBOARD_IRQ, (void *)(keyboard_irq_handler));
    printk(KERN_INFO "Custom keyboard IRQ handler removed\n");
}

module_init(hi);
module_exit(hello);

MODULE_LICENSE("GPL");

