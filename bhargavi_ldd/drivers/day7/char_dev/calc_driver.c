#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "calc_driver"
#define BUF_SIZE 256

static int major;
static char device_buffer[BUF_SIZE];
static int result_ready = 0;

/* ---- Function Prototypes ---- */
static int my_open(struct inode *inode, struct file *file);
static int my_release(struct inode *inode, struct file *file);
static ssize_t my_read(struct file *file, char __user *buf, size_t count, loff_t *ppos);
static ssize_t my_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos);

/* ---- File Operations ---- */
static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = my_open,
    .release = my_release,
    .read = my_read,
    .write = my_write,
};

/* ---- Open ---- */
static int my_open(struct inode *inode, struct file *file)
{
    pr_info("calc_driver: device opened\n");
    return 0;
}

/* ---- Release ---- */
static int my_release(struct inode *inode, struct file *file)
{
    pr_info("calc_driver: device closed\n");
    return 0;
}

/* ---- Read (Send result to user) ---- */
static ssize_t my_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
    if (!result_ready)
        return 0;  // No new data

    if (*ppos > 0)
        return 0; // End of file

    if (copy_to_user(buf, device_buffer, strlen(device_buffer)))
        return -EFAULT;

    *ppos = strlen(device_buffer);
    result_ready = 0;

    return strlen(device_buffer);
}

/* ---- Write (Receive expression from user) ---- */
static ssize_t my_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
    char input[BUF_SIZE];
    int a, b;
    char op;
    int result;

    if (count >= BUF_SIZE)
        return -EINVAL;

    if (copy_from_user(input, buf, count))
        return -EFAULT;

    input[count] = '\0';

    // Expected format: a op b
    if (sscanf(input, "%d %c %d", &a, &op, &b) != 3)
        return -EINVAL;

    switch (op)
    {
    case '+':
        result = a + b;
        break;
    case '-':
        result = a - b;
        break;
    case '*':
        result = a * b;
        break;
    case '/':
        if (b == 0)
            return -EINVAL;
        result = a / b;
        break;
    default:
        return -EINVAL;
    }

    snprintf(device_buffer, BUF_SIZE, "Result = %d\n", result);
    result_ready = 1;

    return count;
}

/* ---- Module Init ---- */
static int __init calc_init(void)
{
    major = register_chrdev(0, DEVICE_NAME, &fops);
    if (major < 0)
    {
        pr_err("calc_driver: failed to register device\n");
        return major;
    }

    pr_info("calc_driver loaded with major %d\n", major);
    return 0;
}

/* ---- Module Exit ---- */
static void __exit calc_exit(void)
{
    unregister_chrdev(major, DEVICE_NAME);
    pr_info("calc_driver unloaded\n");
}

module_init(calc_init);
module_exit(calc_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("You");
MODULE_DESCRIPTION("Simple Calculator Character Driver");

