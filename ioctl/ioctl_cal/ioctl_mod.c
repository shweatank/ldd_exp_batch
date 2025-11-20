#include <linux/kernel.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/ioctl.h>

#define DEVICE_NAME "ioctl_c"
#define MAJOR_NUM 100
#define CALC_MAGIC 'c'
#define IOCTL_CALC _IOWR(CALC_MAGIC, 0, struct calc_data)

struct calc{
    int a;
    int b;
    char ops;
    int result;
};

struct calc cal;
static int my_open(struct inode *inode, struct file *file) {
    pr_info("ioctl_c: device opened\n");
    return 0;
}

static int my_release(struct inode *inode, struct file *file) {
    pr_info("ioctl_c: device closed\n");
    return 0;
}

static long my_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    if(copy_from_user(&cal,(struct calc*)arg,sizeof(cal)))
        return -EFAULT;

    printk("ioctl_c: reading in kernel space %d %d %d\n",cal.a,cal.b,cal.ops);
    switch (cal.ops)
    {
    case '+': cal.result=cal.a+cal.b;
        break;
    case '-': cal.result=cal.a-cal.b;
        break;
    case '*': cal.result=cal.a*cal.b;
        break;
    case '/': if(cal.b==0) return -EINVAL;
                cal.result=cal.a/cal.b;
        break;
    default: 
                return -EINVAL;
        
    }
    if(copy_to_user((struct calc*)arg,&cal,sizeof(cal)))
    {
        return -EINVAL;
    }
    return 0;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = my_open,
    .release = my_release,
    .unlocked_ioctl = my_ioctl,
};


static int __init ioctl_init(void)
{
    int ret = register_chrdev(MAJOR_NUM, DEVICE_NAME, &fops);
    if (ret < 0) {
        pr_err("Failed to register char device\n");
        return ret;
    }

    pr_info("ioctl_c: module loaded, major=%d\n", MAJOR_NUM);
    return 0;
}

static void __exit ioctl_exit(void)
{
    unregister_chrdev(MAJOR_NUM, DEVICE_NAME);
    pr_info("ioctl_c: module unloaded\n");
}

module_init(ioctl_init);
module_exit(ioctl_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("swapnil");
MODULE_DESCRIPTION("Simple IOCTL driver example");

