#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <asm/io.h>

#define KEYBOARD_IRQ	1
#define KBD_DATA_PORT	0x60

irqreturn_t keyboard_irq_handler(int irq, void *dev_id);

irqreturn_t keyboard_irq_handler(int irq, void *dev_id) {
	unsigned char scancode;
	scancode = inb(KBD_DATA_PORT);

	pr_info("Keyboard IRQ: Scancode = 0x%02X\n", scancode);

	return IRQ_HANDLED;
}

static int __init keyboard_irq_init(void) {
	int result;

	int res = 10/0;
	pr_info("Loading custom keyboard IRQ handler...\n");

	result = request_irq(KEYBOARD_IRQ, keyboard_irq_handler, IRQF_SHARED, "keyboard_irq_handler", (void *)(keyboard_irq_handler));

	if (result) {
		pr_err("Keyboard irq: Cannot register IRQ %d\n", KEYBOARD_IRQ);
		return result;
	}

	pr_info("Keyboard irq: IRQ handler registered successfully\n");
	return 0;
}

static void __exit keyboard_irq_exit(void) {
	free_irq(KEYBOARD_IRQ, (void *)keyboard_irq_handler);
	pr_info("Keyboard irq: IRQ handler removed\n");
}

module_init(keyboard_irq_init);
module_exit(keyboard_irq_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Swathi");
MODULE_DESCRIPTION("Simple driver for interrupt");

