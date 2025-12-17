#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/init.h>
#include<linux/fs.h>

#define DEVICE_NAME "file_operation"
#define BUFF_SIZE 256
static int major;
static char buffer[BUFF_SIZE];
static int buffer_size=0;


static int open_dev(struct inode *inode, struct file *file){
	pr_info("dev is open\n");
	return 0;
}

static int release_dev(struct inode *inode, struct file *file){
	pr_info("dev is close\n");
	return 0;
}

static ssize_t read_dev(struct file *file, char __user *user_buf, size_t count, loff_t *pp){
	if(*pp >= BUFF_SIZE){
		return 0;
	}
	if(count > BUFF_SIZE - *pp){
		count=BUFF_SIZE - *pp;
	}
	if(copy_to_user(user_buf, buffer + *pp, count)){
		return -EFAULT;
	}
	*pp += count;
	pr_info("data read\n");
	return count;
}

static ssize_t write_dev(struct file *file, const char __user *user_buf, size_t count, loff_t *pp){
	if(count > BUFF_SIZE){
		count=BUFF_SIZE;
	}
	if(copy_from_user(buffer, user_buf, count )){
		return -EFAULT;
	}
	buffer_size=count;
	pr_info("data write\n");
	return count;
}



static struct file_operations fops={
	.owner = THIS_MODULE,
	.open = open_dev,
	.release = release_dev,
	.read = read_dev,
	.write = write_dev,
};

static int __init start(void){
	major=register_chrdev(0, DEVICE_NAME, &fops);
	if(major<0){
		pr_err("registration failed\n");
		return major;
	}
	pr_info("register major => %d\n",major);
	return 0;
}

static void __exit end(void){
	unregister_chrdev(major, DEVICE_NAME);
	pr_info("unregister\n");
}

module_init(start);
module_exit(end);
MODULE_LICENSE("GPL");
