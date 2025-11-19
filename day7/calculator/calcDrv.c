#include <linux/module.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/fs.h>

#define DEVICE_NAME "mathops"
#define BUF_SIZE 265

static int major_number;
static int buffer[BUF_SIZE];
static int buffer_size = 0;
static int result = 0;

typedef struct oops{
	int (*padd)(int, int);
	int (*psub)(int, int);
	int (*pmul)(int, int);
	int (*pdiv)(int, int);
}oops;

//function prototype

static int drv_open(struct inode *inode, struct file *file);
static int drv_release(struct inode *inode, struct file *file);
static ssize_t drv_read(struct file *file, char __user *user_buf, 
			size_t count, loff_t *ppos);
static ssize_t drv_write(struct file *file, const char __user *user_buf, 
			 size_t count, loff_t *ppos);
int add(int, int);
int sub(int, int);
int mul(int, int);
int div(int, int);

//Function definations
int add(int a, int b){
	return a+b;
}

int sub(int a, int b){
	return a-b;
}

int mul(int a, int b){
	return a*b;
}

int div(int a, int b){
	return a/b;
}

oops func = {
	.padd = add,
	.psub = sub,
	.pmul = mul,
	.pdiv = div	
};

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
	
	buffer[0] = result;
	//len = snprintf(newbuf,sizeof(newbuf), "%d", result);
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
	
	int a = buffer[1];
	int b = buffer[2];
	int op = buffer[0];

	printk("the numbers %d %d %d",a,b,op);

	if((buffer[0]) == 1){
		printk("operation performed add\n");	
		result = func.padd(a, b);
		printk("result %d",result);

	}else if((buffer[0]) == 2){
		
		result = func.psub(a, b);
	
	}else if((buffer[0]) == 3){
		
		result = func.pmul(a, b);
	
	}else if((buffer[0]) == 4){

		result = func.pdiv(a, b);
	
	}

	buffer_size = count;
	pr_info("Data Written\n");
	return count;	
}

struct file_operations fops = {
	.owner   = THIS_MODULE,
	.open    = drv_open,
	.release = drv_release,
	.read    = drv_read,
	.write   = drv_write
};

static int __init mathops_init(void){
	
	pr_info("Driver init\n");
	major_number = register_chrdev(0, DEVICE_NAME, &fops);
	
	if(major_number < 0){
		pr_err("Registration failed\n");
		return major_number;
	}

	pr_info("driver loaded with maj number %d\n",major_number);
	pr_info("mknod /dev/%s c %d 0\n", DEVICE_NAME, major_number);
	//0 -> successful init for module
	return 0;
}

//module clean-up entry point
static void __exit mathops_exit(void){
	unregister_chrdev(major_number, DEVICE_NAME);
	pr_info("driver exit\n");
}

module_init(mathops_init);
module_exit(mathops_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Devashree Katarkar");
MODULE_DESCRIPTION("Basic driver for math operations");
