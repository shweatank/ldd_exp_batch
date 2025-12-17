#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "simple_char_dev"
#define BUFF_SIZE 256

static int major;
static char buffer[BUFF_SIZE];
static int buffer_size = 0;

// Prototype
static int my_open(struct inode *inode, struct file *file);
static int my_release(struct inode *inode, struct file *file);
static ssize_t my_read(struct file *file, char __user *user_buf, size_t count, loff_t *ppos);
static ssize_t my_write(struct file *file, const char __user *user_buf, size_t count, loff_t *ppos);

// Open
static int my_open(struct inode *inode, struct file *file){
	pr_info("Device open");
	return 0;
}

// Release
static  int my_release(struct inode *inode, struct file *file){
	pr_info("Device close");
	return 0;
}

// Read
static ssize_t my_read(struct file *file, char __user *user_buf, size_t count, loff_t *ppos){
	if(*ppos >= buffer_size){
		return 0;
	}
	if(count > buffer_size - *ppos){
		count = buffer_size - *ppos;
	}
	if(copy_to_user(user_buf, buffer + *ppos, count)){
		return  -EFAULT;
	}
	*ppos += count;
	pr_info("Data Read\n");
	return count;	
}

// writing
static ssize_t my_write(struct file *file, const char __user *user_buf, size_t count, loff_t *ppos){
	if(count > BUFF_SIZE){
		count= BUFF_SIZE;
	}
	if(copy_from_user(buffer, user_buf, count)){
		return -EFAULT;
	}
	buffer_size = count;
	pr_info("Data written\n");
	return count;
}

// File ops
static struct file_operations fops={
	.owner = THIS_MODULE,
	.open = my_open,
	.release = my_release,
	.read = my_read,
	.write = my_write,

};

static int __init simple_init(void){
	major = register_chrdev(0, DEVICE_NAME, &fops);
	if(major < 0){
		pr_err("registratin failed\n");
		return major;
	}
	pr_info("simple_char_dev loadded, major=%d\n",major);
	pr_info("mknode /dev/%s c %d 0\n", DEVICE_NAME, major);
	return 0;
}

static void __exit simple_exit(void){
	unregister_chrdev(major, DEVICE_NAME);
	pr_info("simple_char_dev unloadded\n");
}

module_init(simple_init);
module_exit(simple_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Tchdabba");
MODULE_DESCRIPTION("very basic read/ eritr char device driver");


