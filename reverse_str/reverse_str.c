#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/fs.h>
#include<linux/uaccess.h>
#include<linux/string.h>

#define DEVICE_NAME "revese_str"
#define BUF_SIZE 128

static int major;
static char buffer[BUF_SIZE];
static char buffer_size = 0;
static int my_open(struct inode *inode, struct file *file){
	pr_info("reverse: device closed\n");
	return 0;
}

static int my_release(struct inode *inode, struct file *file){
	pr_info("reverse: device closed\n");
	return 0;
}

static void reverse_string(char *str){
	int i=0;
	int j=strlen(str) - 1;
	while(i<j){
		char tmp=str[i];
		str[i]=str[j];
		str[j]=tmp;

		i++;
		j--;
	}
}

static ssize_t my_read(struct file *file, char __user *user_buf, size_t count, loff_t *ppos){
	if(*ppos >= buffer_size){
		return 0;
	}
	if(copy_to_user(user_buf, buffer, buffer_size)){
		return -EFAULT;
	}

	*ppos +=buffer_size;
	pr_info("data read\n");
	return buffer_size;
}

static ssize_t my_write(struct file *file, const char __user *user_buf, size_t count, loff_t *ppos){
	if(count > BUF_SIZE){
		count = BUF_SIZE;
	}
	if(copy_from_user(buffer, user_buf, count)){
		return -EFAULT;
	}
	buffer[count]='\0';
	reverse_string(buffer);
	pr_info("string reverse\n");
	buffer_size = count;
	return count;
}

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = my_open,
	.release = my_release,
	.read = my_read,
	.write = my_write,
};

static int __init rev_init(void){
	major = register_chrdev(0, DEVICE_NAME, &fops);
	if(major < 0){
		pr_err("registration failed\n");
		return major;
	}
	pr_info("reverse link list, major = %d\n",major);
	return 0;
}

static void __exit rev_exit(void){
	unregister_chrdev(major, DEVICE_NAME);
	pr_info("reverse string unloaded\n");
}

module_init(rev_init);
module_exit(rev_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Pawan");
MODULE_DESCRIPTION("reverse string driver");
