#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <asm/io.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define KEYBOARD_IRQ 1
#define KBD_DATA_PORT 0x60

#define DEVICE_NAME "kbd_calci"
#define BUF_SIZE 64

static char operator ;     
static int a, b;                
static char result[BUF_SIZE];  
static int result_size = 0;

static int major;


static irqreturn_t keyboard_irq_handler(int irq, void *dev_id)
{
    unsigned char sc = inb(KBD_DATA_PORT);

    switch (sc) {

        case 0x0C: operator = '-'; break;     
        case 0x0D: operator = '+'; break;     
        case 0x37: operator = '*'; break;     
        case 0x35: operator = '/'; break;     
        default:
            return IRQ_NONE;
    }

    printk(KERN_INFO "operator detected: %c\n", operator);
    return IRQ_HANDLED;
}


static ssize_t my_write(struct file *file, const char __user *user_buf, size_t count, loff_t *ppos){
    char buf[BUF_SIZE];

    if (count >= BUF_SIZE) count = BUF_SIZE - 1;

    if (copy_from_user(buf, user_buf, count))
        return -EFAULT;

    buf[count] = '\0';

    if (sscanf(buf, "%d %d", &a, &b) != 2)
        return -EINVAL;

    printk(KERN_INFO "numbers received: %d and %d\n", a, b);

    return count;
}


static ssize_t my_read(struct file *file, char __user *user_buf, size_t count, loff_t *ppos){
    if (*ppos > 0)
        return 0;

    switch (operator) {
        case '+': result_size = snprintf(result, BUF_SIZE, "%d\n", a + b); break;
        case '-': result_size = snprintf(result, BUF_SIZE, "%d\n", a - b); break;
        case '*': result_size = snprintf(result, BUF_SIZE, "%d\n", a * b); break;
        case '/':
            if (b == 0)
                result_size = snprintf(result, BUF_SIZE, "ERROR: divide by zero\n");
            else
                result_size = snprintf(result, BUF_SIZE, "%d\n", a / b);
            break;

        default:
            result_size = snprintf(result, BUF_SIZE, "No operator pressed\n");
    }

    if (copy_to_user(user_buf, result, result_size))
        return -EFAULT;

    *ppos = result_size;
    return result_size;
}


static struct file_operations fops = {
    .owner  = THIS_MODULE,
    .read   = my_read,
    .write  = my_write,
};


static int __init kbd_calci_init(void)
{
    int ret;

    major = register_chrdev(0, DEVICE_NAME, &fops);
    printk("Loaded: major=%d\n", major);

    ret = request_irq(KEYBOARD_IRQ, keyboard_irq_handler, IRQF_SHARED,
                      "kbd_calci", (void*)keyboard_irq_handler);

    if (ret) {
        printk("Cannot register IRQ\n");
        return ret;
    }

    printk(KERN_INFO "Keyboard IRQ registered\n");
    return 0;
}

static void __exit kbd_calci_exit(void)
{
    free_irq(KEYBOARD_IRQ, (void*)keyboard_irq_handler);
    unregister_chrdev(major, DEVICE_NAME);
    printk(KERN_INFO "Module removed\n");
}

module_init(kbd_calci_init);
module_exit(kbd_calci_exit);

MODULE_LICENSE("GPL");

