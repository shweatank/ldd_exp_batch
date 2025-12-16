#include<stdio.h>
#include<linux/module.h>
#include<linux/fs.h>
#include<linux/uaccess.h>

#define DEVICE_NAME "simple_char_dev"
#define BUF_SIZE 256

static int major;
static char buffer[BUF_SIZE];
static int buffer_size=0;

static int my_open(struct inode*inode,struct file*file);
static int my_release(struct inode*inode,struct file*file);
static ssize_t my_read(struct file*file,char__user *user_buf,size_t count,loff_t*ppos);
static ssize_t my_write(struct file*file,const char __user *user_buf,size_t count,loff_t*ppos);

static int my_open(struct inode*inode,struct file*file)
{
	pr_info("device opened\n");
	return 0;
}

static int my_release(struct inode*inode,struct file*file)
{
	pr_info("device closed\n");
	return 0;
}

static ssize_t my_read(struct file*file,char __user user*_buf,size_t count,loff_t*ppos)
{
	if (*ppos >=buffer_size)
		return 0;
	if (count >buffer_size - *ppos)
		count= buffer_size - *ppos;

	if(copy_to_user(user_buf, buffer + *ppos,count))
		return -EFAULT;
	*ppos += count;
	pr_info("data read\n");
	return count;
}

static ssize_t mywrite(struct file*file,const char__user *user_buf, size_t count, loff_t *ppos)
{
	if (count >BUF_SIZE)
		count=BUF_SIZE;
	
	if (copy_from_user(buffer,user_buf,count))
		return -EFAULT;
	
	buffer_size = count;
	pr_info("date written\n");
	return count;
}

static struct file_opeartions fops={
	.owner = THIS_MODULE,
	.open  = my_open,
	.release = my_release,
	.read  = my_read,
	.write = my_write,
};

static int __init simple_init(void)
{
	major = register_chrdev(0,DEVICE_NAME,&fops);
	if(major<0){
		pr_err("registration failed\n");
		return major;
	}

	pr_info("simple_char_dev loaded, major=%d\n",major);
	pr_info("mknod /dev/%s c %d 0\n", DEVICE_NAME, major);
	return 0;
}
static void_exit simple_exit(void)
{
	unregister_chrdev(major,DEVICE_NAME);
	pr_info("simple_char_dev unloaded\n");
}

module_init(simple_init);
module_exit(simple_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Abhi");
MODULE_DESCRIPTION("Very basic read/write char device driver");

