#include <linux/init.h>
#include <linux/fs.h>
#include <linux/module.h>
#include <linux/ioctl.h>
#include <linux/kernel.h>

#define DEVICE_NAME "ioctl_calc"
#define MAJOR_NUM	100

#define IOCTL_SET_OPER _IOW(MAJOR_NUM, 0, char)

static int buffer[2];
static int result;
static int operator;

static int my_open(struct inode *inode, struct file *file);
static int my_release(struct inode *inode, struct file *file);
static long my_ioctl(struct file *file, unsigned int cmd, unsigned long arg);
static ssize_t my_read(struct file *file, char __user *user_buf, size_t count, loff_t *ppos);
static ssize_t my_write(struct file *file, const char __user *user_buf, size_t count, loff_t *ppos);

static int my_open(struct inode *inode, struct file *file) {
	pr_info("ioctl demo: Device opened\n");
	return 0;
}

static int my_release(struct inode *inode, struct file *file) {
	pr_info("ioctl demo: Device closed\n");
	return 0;
}

static ssize_t my_read(struct file *file, char __user *user_buf, size_t count, loff_t *ppos) {
	if(count > sizeof(int))
		count = sizeof(int);

	if(copy_to_user(user_buf, (char *)&result, count)) 
		return -EFAULT;

	pr_info("Read result : %d\n", result);
	return count;
}

static ssize_t my_write(struct file *file, const char __user *user_buf, size_t count, loff_t *ppos) {
	if (count > sizeof(buffer)) {
		count = sizeof(buffer);
	}

	if (copy_from_user((char *)buffer, user_buf, count))
		return -EFAULT;

	pr_info("Written numbers %d %d\n", buffer[0], buffer[1]);
	return count;	
}


static long my_ioctl(struct file *file, unsigned int cmd, unsigned long arg) {
	char user_val;

	switch(cmd) {
		case IOCTL_SET_OPER:
			if (copy_from_user(&user_val, (char __user *)arg, sizeof(user_val)))
				return -EFAULT;
			operator = user_val;
			pr_info("ioctl demo: Received Operator = %c\n", operator);
			break;
		default:
			return -EINVAL;
	}

	switch(operator) {
		case '+':
			result = buffer[0] + buffer[1];
			break;
		case '-':
			result = buffer[0] - buffer[1];
			break;
		case '*':
			result = buffer[0] * buffer[1];
			break;
		case '/':
			result = buffer[0] / buffer[1];
			break;
	}
	return 0;
}

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = my_open,
	.release = my_release,
	.read = my_read,
	.write = my_write,
	.unlocked_ioctl = my_ioctl,
};

static int __init ioctl_init(void) {
	int ret = register_chrdev(MAJOR_NUM, DEVICE_NAME, &fops);
	if (ret < 0) {
		pr_err("Failed to regiter char dev\n");
		return ret;
	}
	pr_info("ioctl demo: Module loaded, Major %d\n", MAJOR_NUM);
	return 0;
}

static void __exit ioctl_exit(void) {
	unregister_chrdev(MAJOR_NUM, DEVICE_NAME);
	pr_info("ioctl demo : Module Unloaded\n");
}

module_init(ioctl_init);
module_exit(ioctl_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Swathi");
MODULE_DESCRIPTION("Simple ioctl driver explained\n");
