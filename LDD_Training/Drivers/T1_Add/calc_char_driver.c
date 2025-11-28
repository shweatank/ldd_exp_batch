#include "depend.h"
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/kernel.h>

#define DEVICE_NAME	"calc_char_dev"

struct data {
	int n1;
	int n2;
	char op;
};

#define BUF_SIZE	20

static int major;
static struct data buffer;
int result;

/*Prototypes*/
static int my_open(struct inode *inode, struct file *file);
static int my_release(struct inode *inode, struct file *file);
static ssize_t my_read(struct file *file, char __user *user_buf, size_t count, loff_t *ppos);
static ssize_t my_write(struct file *file, const char __user *user_buf, size_t count, loff_t *ppos);


/* Open */
static int my_open(struct inode *inode, struct file *file) {
	open_print();
	return 0;
}

/* Close */
static int my_release(struct inode *inode, struct file *file) {
	close_print();
	return 0;
}

/* Read */
static ssize_t my_read(struct file *file, char __user *user_buf, size_t count, loff_t *ppos) {
	if (count > sizeof(int))
		count = sizeof(int);

	if (copy_to_user(user_buf,(char *)&result, count))
		return -EFAULT;

	pr_info("Read Result = %d\n", result);
	return count;
}

/* Write */
static ssize_t my_write(struct file *file, const char __user *user_buf, size_t count, loff_t *ppos) {
	if (count > sizeof(struct data))
		count = sizeof(struct data);

	if (copy_from_user((char *)&buffer, user_buf, count))
		return -EFAULT;

	pr_info("%d %c %d\n", buffer.n1, buffer.op, buffer.n2);

	switch (buffer.op) {
		case '+':
			result = buffer.n1 + buffer.n2;
			break;
		case '-':
			result = buffer.n1 - buffer.n2;
			break;
		case '*':
			result = buffer.n1 * buffer.n2;
			break;
		case '/':
			result = buffer.n1 / buffer.n2;
			break;
	}

	pr_info("Written data %d %c %d\n", buffer.n1, buffer.op, buffer.n2);
	return count;
}

/*File ops*/
static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = my_open,
	.release = my_release,
	.read = my_read,
	.write = my_write,
};

static int __init my_init(void) {
	major = register_chrdev(0, DEVICE_NAME, &fops);
	if (major < 0) {
		pr_err("Registration failed\n");
		return major;
	}
	pr_info("simple_char_dev loaded, major=%d\n", major);
	pr_info("mknod /dev/%s c %d 0\n", DEVICE_NAME, major);
	return 0;
}

static void __exit my_exit(void) {
	unregister_chrdev(major, DEVICE_NAME);
	pr_info("simple_char_dev unloaded\n");
	return;
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Swathi");
MODULE_DESCRIPTION("Basic read write char driver");
