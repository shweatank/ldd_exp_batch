#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/pwm.h>
#include <linux/kernel.h>
#include <linux/ioctl.h>

#define DEVICE_NAME "ioctl_led_pwm"
#define MAJOR_NUM    100
#define IOCTL_SET_DT _IOW(MAJOR_NUM, 0, int)  // Set duty cycle (0-100)

// PWM parameters
static struct pwm_device *pwm = NULL;
static u32 period_ns = 20000000;  // 20 ms = 50 Hz
static u32 pwm_on_time = 10000000;

// ----------------- File Operations -----------------
static int my_open(struct inode *inode, struct file *file) {
    pr_info("ioctl demo: Device opened\n");
    return 0;
}

static int my_release(struct inode *inode, struct file *file) {
    pr_info("ioctl demo: Device closed\n");
    return 0;
}

static long my_ioctl(struct file *file, unsigned int cmd, unsigned long arg) {
    u32 duty;

    switch (cmd) {
        case IOCTL_SET_DT:
            if (copy_from_user(&duty, (u32 __user *)arg, sizeof(u32)))
                return -EFAULT;
            if (duty > 100) duty = 100;

            pwm_on_time = period_ns * duty / 100;

            if (!IS_ERR(pwm)) {
                pwm_config(pwm, pwm_on_time, period_ns);
                pr_info("PWM duty set to %u%%\n", duty);
            }
            break;

        default:
            pr_info("Invalid IOCTL command\n");
            return -EINVAL;
    }

    return 0;
}

static struct file_operations fops = {
    .owner          = THIS_MODULE,
    .open           = my_open,
    .release        = my_release,
    .unlocked_ioctl = my_ioctl,
};

// ----------------- Module Init/Exit -----------------
static int __init basic_init(void) {
    int ret;

    // Register character device
    ret = register_chrdev(MAJOR_NUM, DEVICE_NAME, &fops);
    if (ret < 0) {
        pr_err("Failed to register char device\n");
        return ret;
    }

    // Get PWM device (NULL = first available PWM)
    pwm = pwm_get(NULL, NULL);
    if (IS_ERR(pwm)) {
        pr_err("Failed to get any PWM\n");
        unregister_chrdev(MAJOR_NUM, DEVICE_NAME);
        return PTR_ERR(pwm);
    }

    // Configure and enable PWM
    pwm_config(pwm, pwm_on_time, period_ns);
    pwm_enable(pwm);

    pr_info("%s: Module loaded, Major %d\n", DEVICE_NAME, MAJOR_NUM);
    return 0;
}

static void __exit basic_exit(void) {
    if (!IS_ERR(pwm)) {
        pwm_disable(pwm);
        pwm_put(pwm);
    }

    unregister_chrdev(MAJOR_NUM, DEVICE_NAME);
    pr_info("Module unloaded\n");
}

module_init(basic_init);
module_exit(basic_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("TECHDHBA");
MODULE_DESCRIPTION("PWM LED driver with IOCTL control");

