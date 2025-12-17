#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/pwm.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include "pwm_driver.h"

#define DEFAULT_PWM_CHIP   0
#define DEFAULT_PWM_CHANNEL 0
#define DEFAULT_PERIOD_NS 1000000UL

static int pwm_chip = DEFAULT_PWM_CHIP;
static int pwm_channel = DEFAULT_PWM_CHANNEL;
static unsigned long default_period_ns = DEFAULT_PERIOD_NS;

module_param(pwm_chip, int, 0444);
MODULE_PARM_DESC(pwm_chip, "PWM chip number (pwmchipN)");
module_param(pwm_channel, int, 0444);
MODULE_PARM_DESC(pwm_channel, "PWM channel on chip (pwmN)");
module_param(default_period_ns, ulong, 0444);
MODULE_PARM_DESC(default_period_ns, "Default PWM period in ns");

static dev_t dev_num;
static struct cdev pwm_cdev;
static struct class *pwm_class;
static struct device *pwm_device;

static struct pwm_device *pwm;
static struct pwm_state state;
static int major;

/* buffer sizes */
#define WRITE_BUF_SIZE 32
#define READ_BUF_SIZE 32

/* Convert percent (0-100) to duty cycle in ns for current state.period */
static unsigned long percent_to_duty_ns(int percent, unsigned long period_ns)
{
    return (period_ns * percent) / 100;
}

/* /dev write: accepts ASCII percent (e.g., "75\n") */
static ssize_t pwm_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
    char kbuf[WRITE_BUF_SIZE];
    int ret;
    int percent;
    unsigned long duty_ns;

    if (count == 0 || count >= sizeof(kbuf))
        return -EINVAL;

    if (copy_from_user(kbuf, buf, count))
        return -EFAULT;
    kbuf[count] = '\0';

    ret = kstrtoint(strim(kbuf), 10, &percent);
    if (ret)
        return ret;

    if (percent < 0 || percent > 100)
        return -EINVAL;

    pwm_get_state(pwm, &state);

    duty_ns = percent_to_duty_ns(percent, state.period);
    state.duty_cycle = duty_ns;
    state.enabled = (percent != 0); /* if percent 0 -> keep disabled or set duty 0 and enabled = false */
    pwm_apply_state(pwm, &state);

    pr_info("%s: set duty %d%% (%luns)\n", DRIVER_NAME, percent, duty_ns);

    return count;
}

/* /dev read: returns current duty percent as ASCII */
static ssize_t pwm_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
    char out[READ_BUF_SIZE];
    int len;
    unsigned long duty_ns, period_ns;
    int percent;
    int ret;

    pwm_get_state(pwm, &state);
    duty_ns = state.duty_cycle;
    period_ns = state.period ? state.period : default_period_ns;

    percent = (int)((duty_ns * 100) / period_ns);

    len = scnprintf(out, sizeof(out), "%d\n", percent);

    if (*ppos >= len)
        return 0;

    if (count < len)
        return -EINVAL;

    ret = copy_to_user(buf, out, len);
    if (ret)
        return -EFAULT;

    *ppos += len;
    return len;
}

static const struct file_operations pwm_fops = {
    .owner = THIS_MODULE,
    .write = pwm_write,
    .read  = pwm_read,
};

static int __init pwm_char_init(void)
{
    int ret;
    dev_t dev;

    pr_info("%s: init (chip=%d channel=%d period=%luns)\n", DRIVER_NAME, pwm_chip, pwm_channel, default_period_ns);

    /* allocate char dev region */
    ret = alloc_chrdev_region(&dev_num, 0, 1, DEVICE_NAME);
    if (ret) {
        pr_err("%s: alloc_chrdev_region failed: %d\n", DRIVER_NAME, ret);
        return ret;
    }
    major = MAJOR(dev_num);

    cdev_init(&pwm_cdev, &pwm_fops);
    pwm_cdev.owner = THIS_MODULE;
    ret = cdev_add(&pwm_cdev, dev_num, 1);
    if (ret) {
        pr_err("%s: cdev_add failed: %d\n", DRIVER_NAME, ret);
        unregister_chrdev_region(dev_num, 1);
        return ret;
    }

    pwm_class = class_create(CLASS_NAME);
    if (IS_ERR(pwm_class)) {
        pr_err("%s: class_create failed\n", DRIVER_NAME);
        cdev_del(&pwm_cdev);
        unregister_chrdev_region(dev_num, 1);
        return PTR_ERR(pwm_class);
    }

    pwm_device = device_create(pwm_class, NULL, dev_num, NULL, DEVICE_NAME);
    if (IS_ERR(pwm_device)) {
        pr_err("%s: device_create failed\n", DRIVER_NAME);
        class_destroy(pwm_class);
        cdev_del(&pwm_cdev);
        unregister_chrdev_region(dev_num, 1);
        return PTR_ERR(pwm_device);
    }

    /* Request the PWM (deprecated API on some kernels but commonly available) */
    pwm = pwm_request(pwm_channel, DRIVER_NAME);
    if (IS_ERR(pwm)) {
        pr_err("%s: pwm_request failed\n", DRIVER_NAME);
        device_destroy(pwm_class, dev_num);
        class_destroy(pwm_class);
        cdev_del(&pwm_cdev);
        unregister_chrdev_region(dev_num, 1);
        return PTR_ERR(pwm);
    }

    /* Initialize state (period and 50% duty) */
    pwm_get_state(pwm, &state);
    state.period = default_period_ns;
    state.duty_cycle = percent_to_duty_ns(50, state.period);
    state.enabled = true;
    pwm_apply_state(pwm, &state);

    pr_info("%s: initialized, /dev/%s major=%d\n", DRIVER_NAME, DEVICE_NAME, major);
    return 0;
}

static void __exit pwm_char_exit(void)
{
    dev_t dev = dev_num;

    /* disable pwm */
    pwm_get_state(pwm, &state);
    state.enabled = false;
    pwm_apply_state(pwm, &state);

    pwm_free(pwm);

    device_destroy(pwm_class, dev);
    class_destroy(pwm_class);
    cdev_del(&pwm_cdev);
    unregister_chrdev_region(dev_num, 1);

    pr_info("%s: exit\n", DRIVER_NAME);
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Bhargavi");
MODULE_DESCRIPTION("Simple PWM char driver: write percent (0-100) to set duty");
module_init(pwm_char_init);
module_exit(pwm_char_exit);

