#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "simple_char"
#define BUF_SIZE 256

static int major;
static char device_buffer[BUF_SIZE];
static int buffer_size = 0;

/* ---- Function Prototypes ---- */
static int my_open(struct inode *inode, struct file *file);
static int my_release(struct inode *inode, struct file *file);
static ssize_t my_read(struct file *file, char __user *buf, size_t count, loff_t *ppos);
static ssize_t my_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos);

/* ---- Open ---- */
static int my_open(struct inode *inode, struct file *file)
{
    pr_info("simple_char: device opened\n");
    return 0;
}

/* ---- Release ---- */
static int my_release(struct inode *inode, struct file *file)
{
    pr_info("simple_char: device closed\n");
    return 0;
}

/* ---- Read ---- */
static ssize_t my_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
    if (*ppos >= buffer_size)
        return 0;

    if (count > buffer_size - *ppos)
        count = buffer_size - *ppos;

    if (copy_to_user(buf, device_buffer + *ppos, count))
        return -EFAULT;

    *ppos += count;
    pr_info("simple_char: %zu bytes read\n", count);
    return count;
}

/* ---- Write ---- */
static ssize_t my_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
    if (count > BUF_SIZE)
        count = BUF_SIZE;

    if (copy_from_user(device_buffer, buf, count))
        return -EFAULT;

    buffer_size = count;
    *ppos = count;

    pr_info("simple_char: %zu bytes written\n", count);
    return count;
}

/* ---- File Operations ---- */
static struct file_operations fops = {
    .owner   = THIS_MODULE,
    .open    = my_open,
    .release = my_release,
    .read    = my_read,
    .write   = my_write,
};

/* ---- Module Init ---- */
static int __init simple_init(void)
{
    major = register_chrdev(0, DEVICE_NAME, &fops);

    if (major < 0) {
        pr_err("simple_char: cannot register device\n");
        return major;
    }

    pr_info("simple_char: loaded, major = %d\n", major);
    pr_info("Create device file: sudo mknod /dev/%s c %d 0\n", DEVICE_NAME, major);
    return 0;
}

/* ---- Module Exit ---- */
static void __exit simple_exit(void)
{
    unregister_chrdev(major, DEVICE_NAME);
    pr_info("simple_char: unloaded\n");
}

module_init(simple_init);
module_exit(simple_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Bhargavi Pallerla");
MODULE_DESCRIPTION("Very simple character driver");

