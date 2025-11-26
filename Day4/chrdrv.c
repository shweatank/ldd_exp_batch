#include<linux/init.h>
#include<linux/module.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/uaccess.h>

static dev_t devid;
static struct cdev *mycdev;
static int my_open(struct inode *inode,struct file *file)
{
    pr_info("open function is invoked\n");
    return 0;
}

static ssize_t my_read(struct file *file,char __user *buf,size_t size,loff_t *of)
{
    pr_info("read function is invoked\n");
    return 0;
}

static ssize_t my_write(struct file *file,const char __user *buf,size_t size,loff_t *of)
{
    pr_info("write function is invoked\n");
    return 0;
}

static int my_close(struct inode *inode,struct file *file)
{
    pr_info("close function is invoked\n"); 
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
    alloc_chrdev_region(&devid,0,1,"sai");
    mycdev=cdev_alloc();
    mycdev->ops=&fops;
    cdev_add(mycdev,devid,1);
    pr_info("init function is invoked:--%d\n",MAJOR(devid));
    return 0;
}

static void __exit hello(void)
{
    pr_info("exit function is invoked\n");
}

module_init(hi);
module_exit(hello);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Basic Character Driver is Invoked");

