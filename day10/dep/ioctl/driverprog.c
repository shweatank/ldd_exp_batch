#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/ioctl.h>
#include "driverprog.h"

#define DEVICE_NAME "ioctl_demo"
#define MAJOR_NUM 100

#define IOCTL_SET_NUM _IOW(MAJOR_NUM, 0, int)
#define IOCTL_GET_NUM _IOR(MAJOR_NUM, 1, int)

static char operation;
static int buffer[10];
static int result;

int my_open(struct inode *inode, struct file *file ){
	pr_info("ioctl_demo: Device Opened\n");
	return 0;
}


int my_close(struct inode *inode, struct file *file){
	pr_info("ioctl_demo: Device Closed\n");
	return 0;
}


ssize_t drv_read(struct file *file, char __user *user_buf, 
			size_t count, loff_t *ppos){
	count = sizeof(result);
	printk("sending result to user : %d\n",result);
	if(copy_to_user(user_buf, &result, count))
		return -EFAULT;
	return count;
}


ssize_t drv_write(struct file *file, const char __user *user_buf, 
				size_t count, loff_t *ppos){

	if(copy_from_user(buffer, user_buf, count))
		return -EFAULT;
	countt++;
	pr_info("count number : %d\n",countt);
	int a = buffer[0];
	int b = buffer[1];
	printk("performing %d %c %d\n",a,operation,b);
	if(operation == '+'){
		result = a+b;
	}else if(operation == '-'){
		result = a-b;
	}else if(operation == '*'){
		result = a*b;
	}else if(operation == '/'){
		result = a/b;
	}

	return count;

}

static long my_ioctl(struct file *file, unsigned int cmd, unsigned long arg){
	
	char user_val;
	

	switch(cmd){
		case IOCTL_SET_NUM:
			if(copy_from_user(&user_val, (int __user *)arg, sizeof(user_val)))
				return -EFAULT;
			operation = user_val;
			printk("ioctl : received %c\n",operation);
			//pr_info("ioctl_demo: Received number = %d\n",device_number);
			break;
		case IOCTL_GET_NUM:
		//	if(copy_to_user((int __user *)arg, &device_number, sizeof(device_number)))
		//		return -EFAULT;
		//	device_number = user_val;
		//	pr_info("ioctl_demo: sent number = %d\n",device_number);
		//	break;
		default:
			return -EINVAL;
	}
	return 0;
}

static struct file_operations fops = {
	.owner   = THIS_MODULE,
	.open    = my_open,
        .release = my_close,
	.read    = drv_read,
	.write   = drv_write,
        .unlocked_ioctl = my_ioctl	       
};

static int __init ioctl_init(void){
	pr_info("Driver Init\n");

	int ret = register_chrdev(MAJOR_NUM, DEVICE_NAME, &fops);
	
	if(ret < 0){
		pr_err("Failed registration\n");
		return ret;
	}

	pr_info("Module loaded with major num : %d\n", MAJOR_NUM);
	return 0;
}

static void __exit ioctl_exit(void){
	unregister_chrdev(MAJOR_NUM, DEVICE_NAME);
	pr_info("Driver Unloaded\n");
}

module_init(ioctl_init);
module_exit(ioctl_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Devashree Katarkar");
MODULE_DESCRIPTION("This is an IOCTL driver");
