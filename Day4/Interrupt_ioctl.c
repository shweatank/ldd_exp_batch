#include<linux/init.h>
#include<linux/ioctl.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/uaccess.h>
#include <linux/interrupt.h>
#include <linux/io.h>

#define MAJOR_NUM 120
#define DEVICE_NAME "ioctl_demo"
#define MAJOR 100

#define IOCTL_OPE_CHARAC _IOW(MAJOR,2,char)
#define IOCTL_SET_NUM _IOW(MAJOR,0,int)
#define IOCTL_GET_NUM _IOR(MAJOR,1,int)
#define SCANCODE_A 0x1E
#define SCANCODE_S 0x1F
#define SCANCODE_M 0x20
#define SCANCODE_M 0x32
#define KEYBOARD_IRQ 1
#define KBD_DATA_PORT 0x60
#define MOUSE_IRQ 40
#define MOUSE_COMMAND_PORT 0x64




struct stu{
	int a;
	int b;
	int result;
}x;
static int device_number=0;

irqreturn_t keyboard_irq_handler(int irq, void *dev_id)
{

    unsigned char scancode;

    scancode = inb(KBD_DATA_PORT);

    /* Print interrupt info */
	printk("it is in interrupt function:--%d\n",x.result);
    /* Check for 'a' key press (make code only) */
    if (scancode == SCANCODE_A) {
         x.result = x.a + x.b;
   	printk("result Interrupt:--%d\n",x.result);
    }

    /* Check for 's' key press */
    else if (scancode == SCANCODE_S) {
        x.result = x.a - x.b;
    }

    return IRQ_HANDLED;
}

irqreturn_t mouse_irq_handler(int irq,void *devid)
{
	printk("entered into the mouse function\n");
	return IRQ_HANDLED;
}
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
	printk("read result:--%d\n",x.result);
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
			printk(KERN_ERR "Entered into the Ioctl command\n");
			copy_from_user(&op,(char __user *)arg,sizeof(op));			
			int r=request_irq(KEYBOARD_IRQ, keyboard_irq_handler,IRQF_SHARED, "keyboardinterrupt",(void *)(keyboard_irq_handler));
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
	int r1=request_irq(MOUSE_IRQ, mouse_irq_handler,IRQF_SHARED, "keyboardinterrupt",(void *)(mouse_irq_handler));
	pr_info("init function is invoked %d:--\n",MAJOR_NUM);
	return 0;
}


static void __exit ioctl_exit(void)
{
	pr_info("exit function is invoked\n");
	free_irq(KEYBOARD_IRQ,(void *)(keyboard_irq_handler));
	free_irq(MOUSE_IRQ,(void *)(mouse_irq_handler));
}

module_init(ioctl_init);
module_exit(ioctl_exit);

MODULE_LICENSE("GPL");

