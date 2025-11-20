#include <linux/init.h>
#include <linux/fs.h>
#include <linux/module.h>
#include <linux/ioctl.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "ioctl_calc_dev"
#define MAJOR_NUM 100

int buffer[2];
static int buffer_size = 0;
#define BUF_SIZE 	20

#define IOCTL_SET_NUM _IOW(MAJOR_NUM, 0, int)
#define IOCTL_GET_NUM _IOW(MAJOR_NUM, 1, int)

static int device_number = 0;

static int my_open(struct inode *inode, struct file *file)
{
     pr_info("ioctl_demo : device opened \n");
     return 0;
}

static int my_release(struct inode *inode, struct file *file)
{
    pr_info("ioctl_demo : device closed \n");
    return 0;
}

static ssize_t my_read(struct file *file, char __user *user_buf, size_t count, loff_t *ppos) {
	if(*ppos >= buffer_size) {
		return 0;
	}
	if (count > buffer_size - *ppos) {
		count = buffer_size - *ppos;
	}

	if(copy_to_user(user_buf, &buffer + *ppos, count)) 
		return -EFAULT;

	*ppos += count;
	pr_info("Read result : %d\n", buffer[0]);
	return count;
}


static ssize_t my_write(struct file *file, const char __user *user_buf, size_t count, loff_t *ppos) {
	if (count > BUF_SIZE) {
		count = BUF_SIZE;
	}

	if (copy_from_user(buffer, user_buf, count))
		return -EFAULT;

	buffer_size = count;
	*ppos = 0;
	pr_info("Written numbers %d %d\n", buffer[0], buffer[1]);
	return count;	
}


static long my_ioctl(struct file *file, unsigned int cmd, unsigned long arg) {
         // int user_val;
          char choice;

          switch(cmd)
           {
           
		case IOCTL_SET_NUM:
                    if(copy_from_user(&choice, (int __user *)arg, sizeof(choice)))
                    return -EFAULT;
           //   device_number = user_val;
                pr_info("ioctl_demo : received number = %d\n", device_number);
                break;
             
            default: 
                 return -EINVAL;
            }
   
   int res;
	switch(choice) {
		case '+':
			res = buffer[0] + buffer[1];
			break;
		case '-':
			res = buffer[0] - buffer[1];
			break;
		case '*':
			res = buffer[0] * buffer[1];
			break;
		case '/':
			res = buffer[0] / buffer[1];
			break;
	}

	buffer[0] = res;
	buffer_size = 4;
	pr_info("3\n");

    return 0;
}



static struct file_operations fops = {
                   .owner = THIS_MODULE,
                   .open = my_open,
                   .release = my_release,
                   .read = my_read,
	           .write = my_write,
                   .unlocked_ioctl = my_ioctl,
};


static int __init ioctl_init(void)
{
    int ret = register_chrdev(MAJOR_NUM, DEVICE_NAME, &fops);
       if(ret < 0){
          pr_err("failed to register char device\n");
          return ret;
        }
  pr_info("ioctl_demo = module loaded , major %d\n",MAJOR_NUM);
  return 0;
}

static void __exit ioctl_exit(void)
{
   unregister_chrdev(MAJOR_NUM, DEVICE_NAME);
   pr_info("ioctl_demo : module unloaded\n");
}


module_init(ioctl_init);
module_exit(ioctl_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("TechDhaba");
MODULE_DESCRIPTION("simple IOCTL Driver Example");
























