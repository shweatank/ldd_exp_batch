#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/cdev.h>
#include<linux/uaccess.h>
#include<linux/fs.h>
struct stu{
	int a;
	int b;
	int result;
	char op[10];
}x;
static dev_t devid;

static struct cdev *mycdev;

char s[100];

int result=0;
static int my_open(struct inode *inode,struct file *file)
{
	pr_info(KERN_ERR "open funcion invoked\n");
	return 0;
}

static int my_close(struct inode *inode,struct file *file)
{
	pr_info(KERN_INFO "close function is invoked\n");
	return 0;
}

static ssize_t my_read(struct file *file,char __user *buf,size_t size,loff_t *of)
{
	copy_to_user(buf,&x,sizeof(x));
	pr_info(KERN_INFO "read function is invoked\n");
	return 0;
}

static ssize_t my_write(struct file *file,const char __user *buf,size_t size,loff_t *of)
{	
	
	copy_from_user(&x,buf,sizeof(x));
	switch(x.op[0]){
	case '+':
			x.result=x.a+x.b;
			printk("add :--%d\n",result);
			break;
	case '-':	x.result=x.a-x.b;
			printk("sub:--%d\n",result);
			break;
	}
	pr_info(KERN_ERR "write function is invoked\n");
	return 0;
}

struct file_operations fops={
	.owner=THIS_MODULE,
	.open=my_open,
	.read=my_read,
	.write=my_write,
	.release=my_close,
};

static int __init hi(void)
{
	alloc_chrdev_region(&devid,0,1,"chrdev");
	printk("init function is invoked\n:--%d\n",MAJOR(devid));
	mycdev=cdev_alloc();
	mycdev->ops=&fops;
	cdev_add(mycdev,devid,1);
	return 0;
}

static void __exit hello(void)
{
	pr_info("exit function is invoked\n");
}

module_init(hi);
module_exit(hello);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A Basic Character Driver\n");
MODULE_AUTHOR("Sai Varshath Konduri");


