#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <asm/io.h>

#define KEYBOARD_IRQ 1
#define KBD_DATA_PORT 0x60
#define MAJOR_NUM 510
#define DEV_NAME "intrr"

#define IOCTL_GET_NUM _IOR('a',1,int)

static char operation;
static int buffer[10];
static int resultt = 0;
static int flag = 1;

irqreturn_t keyboard_irq_handler(int irq, void *dev_id){
	
	unsigned char scancode;
	scancode = inb(KBD_DATA_PORT);
	
	if (flag) {
		switch (scancode) {
			case 0x1E:  // A
				operation = '+';
				break;

			case 0x1F:  // S
				operation = '-';
				break;

			case 0x20:  // D
				operation = '/';
				break;

			case 0x32:  // M
				operation = '*';
				break;

			default:
				// Ignore other keys
				break;
		}

		//flag = 0;
	}

	//printk(KERN_INFO "keyboard irq: scancode = 0x%02X\n",scancode);

	return IRQ_HANDLED;
}

static int drv_open(struct inode *inode, struct file *file ){
	pr_info("ioctl_demo: Device Opened\n");
	return 0;
}

static int drv_close(struct inode *inode, struct file *file){
	pr_info("ioctl_demo: Device Closed\n");
	return 0;
}


static ssize_t drv_read(struct file *file, char __user *user_buf, 
			size_t count, loff_t *ppos){

	count = sizeof(resultt);
	printk("sending result to user : %d\n",resultt);
	if(copy_to_user(user_buf, &resultt, count))
		return -EFAULT;

	return count;
}



static ssize_t drv_write(struct file *file, const char __user *user_buf, 
				size_t count, loff_t *ppos){

	if(copy_from_user(buffer, user_buf, count))
		return -EFAULT;

	int a = buffer[0];
	int b = buffer[1];
	int dummy = 0;
	int result = request_irq(KEYBOARD_IRQ, keyboard_irq_handler, IRQF_SHARED,
				"keyboard irq handler", &dummy);

	if(result){
		printk(KERN_ERR "keyboard_irq: cannot register IRQ %d\n",KEYBOARD_IRQ);
		return result;
	}

	msleep(3000);

	free_irq(KEYBOARD_IRQ, &dummy);
	
	printk("Performing %d %c %d\n",a,operation,b);
		
	if(operation == '+'){
		resultt = a+b;
	}else if(operation == '-'){
		resultt = a-b;
	}else if(operation == '*'){
		resultt = a*b;
	}else if(operation == '/'){
		resultt = a/b;
	}

	printk("The result calculated: %d\n",resultt);
	return count;
}

static long my_ioctl(struct file *file, unsigned int cmd, unsigned long arg){
	
	switch(cmd){
		case IOCTL_GET_NUM:
			if(copy_to_user((int __user *)arg, &resultt, sizeof(resultt)))
				return -EFAULT;
			pr_info("ioctl_demo: sent number = %d\n",resultt);
			break;
		default:
			return -EINVAL;
	}
	return 0;
}

static struct file_operations fops = {
	.owner   = THIS_MODULE,
	.open    = drv_open,
        .release = drv_close,
	.read    = drv_read,
	.write   = drv_write,
        .unlocked_ioctl = my_ioctl	       
};


static int __init keyboard_irq_init(void){
	
	//int result;

	printk(KERN_INFO "Loading custom keyboard IRQ handler..\n");
	
	int ret = register_chrdev(MAJOR_NUM, DEV_NAME, &fops);	
	if(ret < 0){
		
		pr_err("failed registration %d\n",ret);
		return ret;
	}
	

	printk(KERN_INFO "keyboard_irq: IRQ handler registeres successfully\n");
	return 0;
}

static void __exit keyboard_irq_exit(void){
	unregister_chrdev(MAJOR_NUM,DEV_NAME);
	printk(KERN_INFO "keyboard_irq: IRQ handler removed.\n");
}



module_init(keyboard_irq_init);
module_exit(keyboard_irq_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Devashree Katarkar");
MODULE_DESCRIPTION("This is an IOCTL driver");
