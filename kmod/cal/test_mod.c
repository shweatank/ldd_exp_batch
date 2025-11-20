#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "testmod_2"
#define BUF_SIZE 256

struct calculator {
    int a;
    int b;
    char buf[20];
};

static int major;
static char *kernel_buffer;
static size_t data_size;
static struct calculator cal;
static int result;

static ssize_t test_read(struct file *file, char __user *user_buf, size_t count, loff_t *ppos)
{
    size_t bytes;

    if (*ppos >= data_size)
        return 0;

    bytes = min(count, data_size - *ppos);

    if (copy_to_user(user_buf, kernel_buffer + *ppos, bytes))
        return -EFAULT;

    *ppos += bytes;
    return bytes;
}

static ssize_t test_write(struct file *file, const char __user *user_buf, size_t count, loff_t *ppos)
{
    size_t bytes_to_write = min(count, sizeof(struct calculator));

    if (copy_from_user(&cal, user_buf, bytes_to_write))
        return -EFAULT;

    pr_info("User Input: a=%d b=%d op=%s\n", cal.a, cal.b, cal.buf);

    /* Decide operation */
    if (strstr(cal.buf, "add"))
        result = cal.a + cal.b;
    else if (strstr(cal.buf, "sub"))
        result = cal.a - cal.b;
    else if (strstr(cal.buf, "mul"))
        result = cal.a * cal.b;
    else if (strstr(cal.buf, "div"))
        result = cal.b ? (cal.a / cal.b) : 0;
    else
        result = 0;

    /* Copy result for read() */
    memcpy(kernel_buffer, &result, sizeof(result));
    data_size = sizeof(result);

    return bytes_to_write;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .read = test_read,
    .write = test_write,
};

static int __init testmod_init(void)
{
    kernel_buffer = kmalloc(BUF_SIZE, GFP_KERNEL);
    if (!kernel_buffer)
        return -ENOMEM;

    major = register_chrdev(0, DEVICE_NAME, &fops);
    pr_info("Driver loaded. Major = %d\n", major);
    pr_info("Create device: sudo mknod /dev/%s c %d 0\n", DEVICE_NAME, major);

    return 0;
}

static void __exit testmod_exit(void)
{
    unregister_chrdev(major, DEVICE_NAME);
    kfree(kernel_buffer);
    pr_info("Driver unloaded\n");
}

module_init(testmod_init);
module_exit(testmod_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Swapnil");
MODULE_DESCRIPTION("Arithmetic calculator driver");
