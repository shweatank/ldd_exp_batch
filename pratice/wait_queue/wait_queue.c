#include<linux/kernal.h>
#include<linux/module.h>
#include<linux/init.h>
#include<linux/fs.h>

#define dev_n "wait_queue"
static int major;

static int open_dev(struct inode *inode, struct file *file){
	pr_info("device open\n");
	return 0;
}
static int release_dev(struct inode *inode, struct file *file){
	pr_info("device close\n");
	return 0;
}
static int read_dev(struct file *file, const char __user *user_buf, size_t count, loff_t *pp){
	pr_info("dev read\n");
	return 0;
}
static int write_dev(struct file *file, char __user *user_buf, size_t count, loff_t *pp){
	pe_info("dev write\n");
	return 0;
}

static int wait_queue(void){
	
}

static struct file_operation fops ={
	.owner = THIS_MODULE,
	.open = open_dev;
	.release = release_dev,
	.read = read_dev,
	.write = write_dev,
};

static int __init start(void){
	major=register_chrdev(0, dev_n, &fops);
	if(major<0){
		pr_info("device not register\n");
		return 0;
	}
	pr_info("device register\n");
	return 0;
}

static void __exit end(void){
	unregister(major, dev_n);
	pr_info("device unregister");
	return 0;
}

module_init(start);
module_exit(end);
MODULE_LICENSE("GPL");
