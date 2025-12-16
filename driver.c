// driver.c
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include "driver.h"

static int major;                
static char msg[BUF_LEN];      
static int msg_len = 0;

static int dev_open(struct inode *inode, struct file *file) {
    printk(KERN_INFO "Device opened\n");
    return 0;
}

static int dev_release(struct inode *inode, struct file *file) {
    printk(KERN_INFO "Device closed\n");
    return 0;
}

static ssize_t dev_read(struct file *file, char __user *buf, size_t len, loff_t *offset) {
    int bytes_read = 0;

    if (*offset >= msg_len)
        return 0;

    while (len && (*offset < msg_len)) {
        put_user(msg[*offset], buf++);
        len--;
        (*offset)++;
        bytes_read++;
    }

    printk(KERN_INFO "Read %d bytes\n", bytes_read);
    return bytes_read;
}

static ssize_t dev_write(struct file *file, const char __user *buf, size_t len, loff_t *offset) {
    if (len > BUF_LEN)
        len = BUF_LEN;

    if (copy_from_user(msg, buf, len))
        return -EFAULT;

    msg_len = len;
    printk(KERN_INFO "Received %d bytes from user\n", msg_len);
    return len;
}

static struct file_operations fops = {
    .open = dev_open,
    .release = dev_release,
    .read = dev_read,
    .write = dev_write
};

static int __init simple_init(void) {
    major = register_chrdev(0, DEVICE_NAME, &fops);

    if (major < 0) {
        printk(KERN_ALERT "Registering char device failed\n");
        return major;
    }

    printk(KERN_INFO "Registered device with major %d\n", major);
    return 0;
}

static void __exit simple_exit(void) {
    unregister_chrdev(major, DEVICE_NAME);
    printk(KERN_INFO "Device unregistered\n");
}

module_init(simple_init);
module_exit(simple_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Abhi");
MODULE_DESCRIPTION("Simple Character Driver");

