#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/ioctl.h>

#define DEVICE_NAME "ioctl_demo"
#define MAJOR_NUM 100

static int result = 0;

#define IOCTL_ADD_NUM     _IOW(MAJOR_NUM, 0, int)
#define IOCTL_SUB_NUM     _IOW(MAJOR_NUM, 1, int)
#define IOCTL_MUL_NUM     _IOW(MAJOR_NUM, 2, int)
#define IOCTL_DIV_NUM     _IOW(MAJOR_NUM, 3, int)
#define IOCTL_GET_RESULT  _IOR(MAJOR_NUM, 4, int)

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

    switch(cmd)
    {
        case IOCTL_ADD_NUM:
            copy_from_user(&user_val, (int __user *)arg, sizeof(user_val));
            result += user_val;
            pr_info("ADD %d → result = %d\n", user_val, result);
            break;

        case IOCTL_SUB_NUM:
            copy_from_user(&user_val, (int __user *)arg, sizeof(user_val));
            result -= user_val;
            pr_info("SUB %d → result = %d\n", user_val, result);
            break;

        case IOCTL_MUL_NUM:
            copy_from_user(&user_val, (int __user *)arg, sizeof(user_val));
            result *= user_val;
            pr_info("MUL %d → result = %d\n", user_val, result);
            break;

        case IOCTL_DIV_NUM:
            copy_from_user(&user_val, (int __user *)arg, sizeof(user_val));
            if (user_val == 0) {
                pr_err("DIVISION BY ZERO ERROR!\n");
                return -EINVAL;
            }
            result /= user_val;
            pr_info("DIV %d → result = %d\n", user_val, result);
            break;

        case IOCTL_GET_RESULT:
            copy_to_user((int __user *)arg, &result, sizeof(result));
            pr_info("GET RESULT = %d\n", result);
            break;

        default:
            return -EINVAL;
    }

    return 0;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open  = my_open,
    .release = my_release,
    .unlocked_ioctl = my_ioctl,
};

static int __init simple_init(void)
{
    int ret = register_chrdev(MAJOR_NUM, DEVICE_NAME, &fops);
    if (ret < 0) {
        pr_err("Registration failed\n");
        return ret;
    }

    pr_info("ioctl_demo loaded, major=%d\n", MAJOR_NUM);
    pr_info("Run: mknod /dev/%s c %d 0\n", DEVICE_NAME, MAJOR_NUM);
    return 0;
}

static void __exit simple_exit(void)
{
    unregister_chrdev(MAJOR_NUM, DEVICE_NAME);
    pr_info("ioctl_demo unloaded\n");
}

module_init(simple_init);
module_exit(simple_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Abhi");
MODULE_DESCRIPTION("IOCTL Driver with ADD/SUB/MUL/DIV Operations");


