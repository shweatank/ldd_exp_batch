#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include "char_driver.h"

#define BUF_LEN 1024

static int major;
static struct class *char_class;
static struct cdev char_cdev;

static char msg_buffer[BUF_LEN];
static int msg_ptr = 0;

static int my_open(struct inode *inode, struct file *file)
{
	pr_info("mychardev: device opened\n");
	return 0;
}

static int my_release(struct inode *inode, struct file *file)
{
	pr_info("mychardev: device closed\n");
	return 0;
}

static ssize_t my_read(struct file *file, char __user *buffer, size_t length, loff_t *offset)
{
	if (*offset >= msg_ptr)
		return 0;

	if (length > msg_ptr - *offset)
		length = msg_ptr - *offset;

	if (copy_to_user(buffer, msg_buffer + *offset, length))
		return -EFAULT;

	*offset += length;
	pr_info("mychardev: data read\n");
	return length;
}

static ssize_t my_write(struct file *file, const char __user *buffer, size_t length, loff_t *offset)
{
	if (length > BUF_LEN)
		length = BUF_LEN;

	if (copy_from_user(msg_buffer, buffer, length))
		return -EFAULT;

	msg_ptr = length;
	pr_info("mychardev: data written\n");
	return length;
}

static struct file_operations fops = {
	.owner   = THIS_MODULE,
	.open    = my_open,
	.release = my_release,
	.read    = my_read,
	.write   = my_write
};

static int __init mychar_init(void)
{
	dev_t dev;

	if (alloc_chrdev_region(&dev, 0, 1, DEVICE_NAME) < 0)
		return -1;

	major = MAJOR(dev);
	pr_info("mychardev: registered with major number %d\n", major);

	cdev_init(&char_cdev, &fops);
	cdev_add(&char_cdev, dev, 1);

	char_class = class_create(THIS_MODULE, CLASS_NAME);
	device_create(char_class, NULL, dev, NULL, DEVICE_NAME);

	return 0;
}

static void __exit mychar_exit(void)
{
	dev_t dev = MKDEV(major, 0);

	device_destroy(char_class, dev);
	class_destroy(char_class);
	cdev_del(&char_cdev);
	unregister_chrdev_region(dev, 1);

	pr_info("mychardev: unregistered driver\n");
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Bhargavi");
MODULE_DESCRIPTION("Simple Character Driver");
MODULE_VERSION("1.0");

module_init(mychar_init);
module_exit(mychar_exit);
