#include <linux/init.h>
#include <linux/ioctl.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/module.h>

#define DEVICE_NAME "ioctl_calci"
#define MAJOR_NUM 100
#define BUF_SIZE 256

#define IOCTL_SET_NUM _IOW(MAJOR_NUM, 0, int)
#define IOCTL_GET_NUM _IOR(MAJOR_NUM, 1, int)

static char buffer[BUF_SIZE];
static char result[BUF_SIZE];
static int result_size = 0;

static int device_number = 0;

static int my_open(struct inode *inode, struct file *file)
{
    pr_info("ioctl_demo: device opened\n");
    return 0;
}

static int my_release(struct inode *inode, struct file *file)
{
    pr_info("ioctl_demo: device closed\n");
    return 0;
}

static long my_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    int user_val;

    switch (cmd) {

    case IOCTL_SET_NUM:
        if (copy_from_user(&user_val, (int __user *)arg, sizeof(user_val)))
            return -EFAULT;

        device_number = user_val;
        pr_info("ioctl_demo: received number = %d\n", device_number);
        break;

    case IOCTL_GET_NUM:
        if (copy_to_user((int __user *)arg, &device_number, sizeof(device_number)))
            return -EFAULT;

        pr_info("ioctl_demo: sent number = %d\n", device_number);
        break;

    default:
        return -EINVAL;
    }

    return 0;
}

static ssize_t my_write(struct file *file, const char __user *user_buf,
                        size_t count, loff_t *ppos)
{
    int a, b;
    char op;

    if (count > BUF_SIZE - 1)
        count = BUF_SIZE - 1;

    if (copy_from_user(buffer, user_buf, count))
        return -EFAULT;

    buffer[count] = '\0';

    if (sscanf(buffer, "%d %d %c", &a, &b, &op) != 3) {
        pr_info("Invalid input format\n");
        result_size = snprintf(result, BUF_SIZE, "Invalid format\n");
        return count;
    }

    pr_info("Received: %d %d %c\n", a, b, op);

    switch (op) {
    case '+':
        result_size = snprintf(result, BUF_SIZE, "%d\n", a + b);
        break;

    case '-':
        result_size = snprintf(result, BUF_SIZE, "%d\n", a - b);
        break;

    case '*':
        result_size = snprintf(result, BUF_SIZE, "%d\n", a * b);
        break;

    case '/':
        if (b == 0)
            result_size = snprintf(result, BUF_SIZE, "Error: divide by zero\n");
        else
            result_size = snprintf(result, BUF_SIZE, "%d\n", a / b);
        break;

    default:
        result_size = snprintf(result, BUF_SIZE, "Error: unknown operator\n");
        break;
    }

    return count;
}

static ssize_t my_read(struct file *file, char __user *user_buf,
                       size_t count, loff_t *ppos)
{
    if (*ppos > 0)
        return 0;

    if (copy_to_user(user_buf, result, result_size))
        return -EFAULT;

    *ppos = result_size;
    return result_size;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = my_open,
    .release = my_release,
    .unlocked_ioctl = my_ioctl,
    .read = my_read,
    .write = my_write,
};

static int __init ioctl_init(void)
{
    int ret;

    ret = register_chrdev(0, DEVICE_NAME, &fops);
    if (ret < 0) {
        pr_err("ioctl_demo: failed to register char device\n");
        return ret;
    }

    pr_info("ioctl_demo: module loaded with major %d\n", ret);
    return 0;
}

static void __exit ioctl_exit(void)
{
    unregister_chrdev(MAJOR_NUM, DEVICE_NAME);
    pr_info("ioctl_demo: module unloaded\n");
}

module_init(ioctl_init);
module_exit(ioctl_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Pawan");
MODULE_DESCRIPTION("ioctl calculator driver");

