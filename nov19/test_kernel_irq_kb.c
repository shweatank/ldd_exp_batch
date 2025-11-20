#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/module.h>
#include <linux/ioctl.h>
#include <linux/uaccess.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/wait.h>
#include <linux/sched.h>

#define DEVICE_NAME "interrupt_calc_dev"
#define MAJOR_NUM 100
static int major = 0;

#define KEYBOARD_IRQ 1
#define KBD_DATA_PORT 0x60
#define A_KEY 0x1e

static int result_ready = 0;
static wait_queue_head_t wq;

int buffer[2];
static int output = 0;

static int buffer_size = 0;
#define BUF_SIZE        20

irqreturn_t keyboard_irq_handler(int irq, void *dev_id)
{
    unsigned char scancode;

    scancode = inb(KBD_DATA_PORT);

    if(scancode == A_KEY)
    {
        output = buffer[1] + buffer[2];

        result_ready = 1;
        wake_up_interruptible(&wq);
       }

    printk(KERN_INFO "keyboard IRQ: Scancode = 0x%02x\n",scancode);
    return IRQ_HANDLED;
}

static ssize_t my_read(struct file *file, char __user *user_buf, size_t count, loff_t *ppos) {
          if(*ppos >= buffer_size) {
                  return 0;
          }
          if (count > buffer_size - *ppos) {
                  count = buffer_size - *ppos;
          }
         
       if(wait_event_interruptible(wq, result_ready))
               return -ERESTARTSYS; 

          if(copy_to_user(user_buf, &output + *ppos, count))
                  return -EFAULT;
  
          *ppos += count;
           result_ready = 0;
         // pr_info("Read result : %d\n", buffer[0]);
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

static const struct file_operations fops = {
    .owner = THIS_MODULE,
    .read = my_read,
    .write = my_write,
};

static int __init keyboard_irq_init(void)
{
    int result,ret;
    
     init_waitqueue_head(&wq);
//     major = register_chrdev(0, DEVICE_NAME, &fops);
  ret = register_chrdev(MAJOR_NUM, DEVICE_NAME, &fops);
   if(ret < 0) {
        printk(KERN_ERR "Failed to register char device\n");
        return major;
    }

    printk(KERN_INFO "loading custom keyboard IRQ Handler ..\n");
    result = request_irq(KEYBOARD_IRQ, keyboard_irq_handler, IRQF_SHARED,
                                    "keyboard_irq_handler", (void *)(keyboard_irq_handler));

   if(result)
        {
          printk(KERN_ERR "keyboard_irq: cannot register IRQ %d\n",KEYBOARD_IRQ);
           return result;
             }
    printk(KERN_INFO "keyboard_irq : IRQ handler registered succesfully\n");
    printk(KERN_INFO "kbd_calc loaded. Major=%d\n", MAJOR_NUM);    
   
return 0;
}


static void __exit keyboard_irq_exit(void) {
   free_irq(KEYBOARD_IRQ, (void*)(keyboard_irq_handler));
   unregister_chrdev(MAJOR_NUM, DEVICE_NAME);
   printk(KERN_INFO "keyboard_irq: IRQ handler removed\n");
}




module_init(keyboard_irq_init);
module_exit(keyboard_irq_exit);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Techdhaba");
MODULE_DESCRIPTION("keyboard IRQ Handler for x86 linux");









