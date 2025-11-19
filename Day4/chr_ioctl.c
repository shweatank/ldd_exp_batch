#include<linux/init.h>
#include<linux/ioctl.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/uaccess.h>

#define MAJOR_NUM 120
#define DEVICE_NAME "ioctl_demo"
#define MAJOR 100

#define IOCTL_OPE_CHARAC _IOW(MAJOR,2,char)
#define IOCTL_SET_NUM _IOW(MAJOR,0,int)
#define IOCTL_GET_NUM _IOR(MAJOR,1,int)

struct stu{
	int a;
	int b;
	int result;
}x;
static int device_number=0;

static int my_open(struct inode *inode,struct file *file)
{
	pr_info("open function is invoked\n");
	return 0;
}

static int my_close(struct inode *inode,struct file *file)
{
	pr_info("close function is invoked\n");
	return 0;
}
static ssize_t my_read(struct file *file,char __user *buf,size_t size,loff_t *of)
{
	copy_to_user(buf,&x,sizeof(x));
	return 0;
}

static ssize_t my_write(struct file *file,const char __user *buf,size_t size,loff_t *of)
{
	copy_from_user(&x,buf,sizeof(x));
	return 0;
}
static long my_ioctl(struct file *file,unsigned int cmd,unsigned long arg)
{
	char op;
	int usr_val;
	switch(cmd)
	{
		case IOCTL_SET_NUM:
			if(copy_from_user(&usr_val,(int __user *)arg,sizeof(usr_val)))
					return -EFAULT;
			device_number=usr_val;
			pr_info("ioctl demo: Received Number= %d\n",device_number);
			break;
		case IOCTL_GET_NUM:
			if(copy_to_user((int __user *)arg,&device_number,sizeof(device_number)))
				return -EFAULT;
			pr_info("sent number:--%d\n",device_number);
			break;
		case IOCTL_OPE_CHARAC:
			copy_from_user(&op,(char __user *)arg,sizeof(op));
			if(op=='+')
				x.result=x.a+x.b;
			break;
		default:
			return -EINVAL;
	}
	return 0;
}


struct file_operations fops={
	.owner=THIS_MODULE,
	.open=my_open,
	.release=my_close,
	.unlocked_ioctl=my_ioctl,
	.read=my_read,
	.write=my_write,
};

static int __init ioctl_init(void)
{
	int ret=register_chrdev(MAJOR_NUM,DEVICE_NAME,&fops);

	pr_info("init function is invoked %d:--\n",MAJOR_NUM);
	return 0;
}


static void __exit ioctl_exit(void)
{
	pr_info("exit function is invoked\n");
}

module_init(ioctl_init);
module_exit(ioctl_exit);

MODULE_LICENSE("GPL");

