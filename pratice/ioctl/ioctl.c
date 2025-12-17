#include<linux/kernal.h>
#include<linux/module.h>
#include<linux/init.h>
#include<linux/ioctl.h>
#include<linux/fs.h>

#define device_n "ioctl_fun"
#define buf_s 5;
static int buffer[buf_s];
static int major;
static int ans;


static int open_dev(struct node *node, struct file *file){
	pr_info("dev open\n");
	return 0;
}
static int release_dev(struct node *node, struct file *file){
	pr_info("dev close\n");
	return 0;
}
struct int read_dev(struct file *file, char __user user_buf, size_t count, loff_t *pp){
	if(count > buf_s){
		count = buf_s;
	}
	if(copy_to_user(user_buf, buffer, count)){
		return -EFAULT;
	}
	return count;
}
struct int write_dev(struct file *file, char __user user_buf, size_t count, loff_t *PP){

}

static int ioctl_fun(struct file *file, unsigned int cmd){
	switch(cmd){
		case '+':
			ans=bufer[0]+bufer[1];
			break;
		case '-':
			ans=bufer[0]+bufer[1];
			break;
		case '*':
			ans=bufer[0]+bufer[1];
			break;
		case '/':
			if(bufer[1] == 0){
				ans=0;
			}
			else{
				ans=buffer[0]/buffer[1];
			}
			break;
		default:
			ans=-1;
			break;
	}
	return 0;
}

static struct file_operation *fops={
	.owner = THIS_MODULE,
	.open = open_dev,
	.release = release_dev,
	.read = read_dev,
	.write = write_dev,
	.unloacked_ioctl = ioctl_fun,
}
static int __init start(void){
	int major=register_chrdev(0, device_n, &fops);
	if(major<0){
		pe_err("device register major number =\t%d",major);
		return major;
	}
	pr_info("device register major number =\t%d",major);
	return 0;
}
static void __exit end(void){
	unregister_chrdev(major, device_n);
	pr_info("device unregister\n");
}

module_inti(start);
module_exit(end);

MODULE_LICENSE("GPL");
