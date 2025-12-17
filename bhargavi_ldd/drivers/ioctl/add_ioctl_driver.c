#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/device.h>

#define DEVICE_NAME "ioctl_add"
#define MAJOR_NUM 240

#define IOCTL_ADD _IOW(MAJOR_NUM, 0, struct add_data)

struct add_data {
    int a;
    int b;
};

static int result = 0;

static int dev_open(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "Device opened\n");
    return 0;
}

static int dev_release(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "Device closed\n");
    return 0;
}

static long dev_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    struct add_data data;

    switch (cmd) {
        case IOCTL_ADD:
            if (copy_from_user(&data, (struct add_data *)arg, sizeof(data)))
                return -EFAULT;

            result = data.a + data.b;
            printk(KERN_INFO "Addition inside kernel: %d + %d = %d\n",
                    data.a, data.b, result);
            break;

        default:
            return -EINVAL;
    }
    return 0;
}

static ssize_t dev_read(struct file *file, char __user *buf,
                        size_t len, loff_t *offset)
{
    if (copy_to_user(buf, &result, sizeof(int)))
        return -EFAULT;

    printk(KERN_INFO "Read result sent to user: %d\n", result);
    return sizeof(int);
}

static struct file_operations fops = {
    .owner          = THIS_MODULE,
    .open           = dev_open,
    .release        = dev_release,
    .unlocked_ioctl = dev_ioctl,
    .read           = dev_read,
};

static int major;
static struct class *cls;

static int __init add_init(void)
{
    major = register_chrdev(0, DEVICE_NAME, &fops);
    printk(KERN_INFO "Device registered with major %d\n", major);

    cls = class_create(THIS_MODULE, "add_class");
    device_create(cls, NULL, MKDEV(major, 0), NULL, DEVICE_NAME);

    return 0;
}

static void __exit add_exit(void)
{
    device_destroy(cls, MKDEV(major, 0));
    class_destroy(cls);
    unregister_chrdev(major, DEVICE_NAME);
}

module_init(add_init);
module_exit(add_exit);

MODULE_LICENSE("GPL");

