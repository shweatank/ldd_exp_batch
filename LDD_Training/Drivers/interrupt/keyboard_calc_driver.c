#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/fs.h>
#include <asm/io.h>

#define DEVICE_NAME "interrupt_calc"
#define MAJOR_NUM	100

#define KEYBOARD_IRQ	1
#define KBD_DATA_PORT	0x60

int buffer[2];
int result;
int result_received = 0;

static int my_open(struct inode *inode, struct file *file);
static int my_release(struct inode *inode, struct file *file);
static ssize_t my_read(struct file *file, char __user *user_buf, size_t count, loff_t *ppos);
static ssize_t my_write(struct file *file, const char __user *user_buf, size_t count, loff_t *ppos);

irqreturn_t keyboard_irq_handler(int irq, void *dev_id);

irqreturn_t keyboard_irq_handler(int irq, void *dev_id) {
	free_irq(KEYBOARD_IRQ, (void *)keyboard_irq_handler);
	unsigned char scancode;
	scancode = inb(KBD_DATA_PORT);

	switch(scancode) {
		case 0x1E:
			result = buffer[0] + buffer[1];
			break;
		case 0x1F:
			result = buffer[0] - buffer[1];
			break;
		case 0x32:
			result = buffer[0] * buffer[1];
			break;
		case 0x20:
			result = buffer[0] / buffer[1];
			break;
	}
	result_received = 1;
	return IRQ_HANDLED;
}

static int my_open(struct inode *inode, struct file *file) {
	pr_info("Device opened\n");
	return 0;
}

static int my_release(struct inode *inode, struct file *file) {
	pr_info("Device closed\n");
	return 0;
}

static ssize_t my_read(struct file *file, char __user *user_buf, size_t count, loff_t *ppos) {
	if(count > sizeof(int)) 
		count = sizeof(int);

	if(!result_received)
		return 0;

	if(copy_to_user(user_buf, (char *)&result, count)) 
		return -EFAULT;
	
	pr_info("Read %d\n", result);
	return count;
}

static ssize_t my_write(struct file *file, const char __user *user_buf, size_t count, loff_t *ppos) {
	if(count > sizeof(buffer)) 
		count = sizeof(buffer);
	
	if (copy_from_user((char *)buffer, user_buf, count))
		return -EFAULT;

	pr_info("Loading custom keyboard IRQ handler...\n");

	result = request_irq(KEYBOARD_IRQ, keyboard_irq_handler, IRQF_SHARED, "keyboard_irq_handler", (void *)keyboard_irq_handler);

	if (result) {
		pr_err("Keyboard irq: Cannot register IRQ %d\n", KEYBOARD_IRQ);
		return result;
	}

	pr_info("Keyboard irq: IRQ handler registered successfully\n");
	pr_info("Written %d and %d\n", buffer[0], buffer[1]);
	return count;
}

static struct file_operations fops = {
     .owner = THIS_MODULE,
     .open = my_open,
     .release = my_release,
     .read = my_read,
     .write = my_write,
};

static int __init keyboard_irq_init(void) {
	int result;
	
	result = register_chrdev(MAJOR_NUM, DEVICE_NAME, &fops);
	if (result <0) {
		pr_err("Failed to register char dev\n");
		return result;
	}
	pr_info("interrupt_calc : Module loaded - Major Number %d\n", MAJOR_NUM);
	return 0;
}

static void __exit keyboard_irq_exit(void) {
	unregister_chrdev(MAJOR_NUM, DEVICE_NAME);
	free_irq(KEYBOARD_IRQ, (void *)keyboard_irq_handler);
	pr_info("interrupt_calc : Module Unloaded - Major Number %d\n", MAJOR_NUM);
}

module_init(keyboard_irq_init);
module_exit(keyboard_irq_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Swathi");
MODULE_DESCRIPTION("Simple driver for interrupt");

