#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/kernel.h>

#define DEVICE_NAME	"reverse_str_char_dev"
#define BUF_SIZE	100

int major;
char string[100];

/*Prototypes*/
static int my_open(struct inode *inode, struct file *file);
static int my_release(struct inode *inode, struct file *file);
static ssize_t my_read(struct file *file, char __user *user_buf, size_t count, loff_t *ppos);
static ssize_t my_write(struct file *file, const char __user *user_buf, size_t count, loff_t *ppos);

/* Open */
static int my_open(struct inode *inode, struct file *file) {
	pr_info("Device opened\n");
	return 0;
}

/* Close */
static int my_release(struct inode *inode, struct file *file) {
	pr_info("Device closed now\n");
	return 0;
}

/* Read */
static ssize_t my_read(struct file *file, char __user *user_buf, size_t count, loff_t *ppos) {
	if (count > BUF_SIZE-1)
		count = BUF_SIZE-1;

	if (copy_to_user(user_buf, string, count))
		return -EFAULT;

	pr_info("Read Result: %s\n", string);
	return count;
}

/* Write */
static ssize_t my_write(struct file *file, const char __user *user_buf, size_t count, loff_t *ppos) {
	if (count > BUF_SIZE-1)
		count = BUF_SIZE-1;

	if (copy_from_user(string, user_buf, count))
		return -EFAULT;
	
	pr_info("Written input: %s\n", string);
	
	string[count] = '\0';
	int i=0;
	int j=count-1;
	
	while(i<j) {
		char temp = string[i];
		string[i] = string[j];
		string[j] = temp;
		i++;
		j--;
	}

	pr_info("Written input: %s\n", string);
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
