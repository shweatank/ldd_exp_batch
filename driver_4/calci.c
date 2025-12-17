#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/fs.h>
#include<linux/uaccess.h>

#define DEVICE_NAME "calci"
#define BUF_SIZE 128

static int major;
static char buffer[BUF_SIZE];
static char result[BUF_SIZE];
static int result_size = 0;

static int my_open(struct inode *inode, struct file *file){
	pr_info("calci: device opened\n");
	return 0;
}

static int my_release(struct inode *inode, struct file *file){
	pr_info("calci: device closed\n");
	return 0;
}

static ssize_t my_write(struct file *file, const char __user *user_buf, size_t count, loff_t *ppos){
	int a, b;
	char op;

	if(count > BUF_SIZE-1){
		count=BUF_SIZE-1;
	}
	if(copy_from_user(buffer, user_buf,count)){
		return -EFAULT;
	}

	buffer[count]='\0';

	if(sscanf(buffer, "%d %d %c", &a, &b, &op) != 3){
		pr_info("invalid input format\n");
		return count;
	}
	pr_info("received: %d %d %c\n",a,b,op);

	switch(op){
		case '+':
			result_size= snprintf(result, BUF_SIZE, "%d\n", a+b);
			break;
		case '-':
			result_size= snprintf(result, BUF_SIZE, "%d\n", a-b);
			break;
		case '*':
			result_size= snprintf(result, BUF_SIZE, "%d\n", a*b);
			break;
		case '/':
			if(b==0){
				result_size= snprintf(result, BUF_SIZE, "Error :divide by zero\n");
			}
			else{
				result_size= snprintf(result, BUF_SIZE, "%d", a/b);
			}
			break;

		default:
			result_size= snprintf(result, BUF_SIZE, "Error: unknown op\n");
			break;

	}
	return count;
}

static ssize_t my_read(struct file *file, char __user *user_buf, size_t count, loff_t *ppos){
	if(*ppos > 0)
		return 0;

	if(copy_to_user(user_buf, result, result_size))
		return -EFAULT;

	*ppos = result_size;
	return result_size;
}


static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = my_open,	
	.release = my_release,
	.read = my_read,
	.write = my_write,
};

static int __init calci_init(void){
	major=register_chrdev(0, DEVICE_NAME, &fops);
	if(major < 0){
		pr_err("failed to register = %d\n",major);
		return major;
	}

	pr_info("calci loaded, major = %d\n",major);
	    pr_info("create device: mknod /dev/%s c %d 0\n", DEVICE_NAME, major);
	    return 0;
}

static void __exit calci_exit(void){
	unregister_chrdev(major, DEVICE_NAME);
	pr_info("calci unloaded\n");
}


module_init(calci_init);
module_exit(calci_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("TECHDHABA");
MODULE_DESCRIPTION("Calculator driver supporting + - * /");




