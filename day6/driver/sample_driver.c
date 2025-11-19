#include <linux/init.h>
#include <linux/fs.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>

#define DEV_NAME "char_drv"
#define BUF_SIZE 256

static int major_num;
static char buffer[BUF_SIZE];
static int buffer_size = 0;

//function prototype

static int drv_open(struct inode *inode, struct file *file);
static int drv_release(struct inode *inode, struct file *file);
static ssize_t drv_read(struct file *file, char __user *user_buf, 
			size_t count, loff_t *ppos);
static ssize_t drv_write(struct file *file, const char __user *user_buf, 
			 size_t count, loff_t *ppos);
//Function definations
static int drv_open(struct inode *inode, struct file *file){
	pr_info("Driver open\n");
	return 0;
}

static int drv_release(struct inode *inode, struct file *file){
	pr_info("Driver release\n");
	return 0;
}

static ssize_t drv_read(struct file *file, char __user *user_buf, 
			size_t count, loff_t *ppos){
	if(*ppos >= buffer_size)
		return 0;

	if(count > buffer_size - *ppos)
		count = buffer_size - *ppos;
	//user performing read, so kernal writing into user_buf
	//so that user can read
	if(copy_to_user(user_buf, buffer + *ppos, count))
		return -EFAULT;
	*ppos += count;
	pr_info("data read\n");
	return count;	
}

static ssize_t drv_write(struct file *file, const char __user *user_buf, 
			 size_t count, loff_t *ppos){
	
	if(count > BUF_SIZE)
		count = BUF_SIZE;
	//when user performs write, we copy data from user into buffer
	//user sends it through user_buf
	if(copy_from_user(buffer, user_buf, count))
		return -EFAULT;
	
	buffer_size = count;
	pr_info("data written\n");
	return count;	
}

struct file_operations fops = {
	.owner   = THIS_MODULE,
	.open    = drv_open,
	.release = drv_release,
	.read    = drv_read,
	.write   = drv_write
};
//module init function
//called during module insertion
//__init - compiler directive tells compiler to keep data in .init section

static int __init my_drv_init(void){
	
	pr_info("Driver init\n");
	major_num = register_chrdev(0, DEV_NAME, &fops);
	
	if(major_num < 0){
		pr_err("Registration failed\n");
		return major_num;
	}

	pr_info("driver loaded with maj number %d\n",major_num);
	pr_info("mknod /dev/%s c %d 0\n", DEV_NAME, major_num);
	//0 -> successful init for module
	return 0;
}

//module clean-up entry point
static void __exit my_drv_exit(void){
	unregister_chrdev(major_num, DEV_NAME);
	pr_info("driver exit\n");
}

module_init(my_drv_init);
module_exit(my_drv_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("devashree katarkar");
MODULE_DESCRIPTION("char driver with fileops");
