#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/pwm.h>
#include <linux/kernel.h>
#include <linux/ioctl.h>
 
#define DEVICE_NAME "ioctl_led_pwm"
#define MAJOR_NUM 0  // Let kernel assign major number dynamically
#define IOCTL_SET_DT _IOW(MAJOR_NUM, 0, int)
 
static int major_num;  // Store dynamically assigned major number
static struct pwm_device *pwm0 = NULL;
static u32 period_ns = 20000000;  // 20 ms = 50 Hz
static u32 duty_cycle = 50;       // Initial duty cycle 50%
 
// File operations
static int my_open(struct inode *inode, struct file *file)
{
    pr_info("ioctl_led_pwm: Device opened\n");
    return 0;
}
 
static int my_release(struct inode *inode, struct file *file)
{
    pr_info("ioctl_led_pwm: Device closed\n");
    return 0;
}
 
static long my_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    u32 duty;
    u32 on_time;
    int ret = 0;
 
    switch (cmd) {
    case IOCTL_SET_DT:
        if (copy_from_user(&duty, (u32 __user *)arg, sizeof(u32)))
            return -EFAULT;
        
        // Validate duty cycle
        if (duty > 100) {
            pr_warn("Duty cycle clamped to 100%% (was %u)\n", duty);
            duty = 100;
        }
        
        duty_cycle = duty;
        on_time = period_ns * duty / 100;
        
        if (!IS_ERR(pwm0)) {
            ret = pwm_config(pwm0, on_time, period_ns);
            if (ret) {
                pr_err("Failed to configure PWM: %d\n", ret);
                return ret;
            }
            pr_info("PWM duty cycle set to %u%%\n", duty);
        } else {
            pr_err("PWM device not available\n");
            return -ENODEV;
        }
        break;
        
    default:
        pr_err("Invalid IOCTL command: 0x%x\n", cmd);
        return -ENOTTY;
    }
    
    return ret;
}
 
static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = my_open,
    .release = my_release,
    .unlocked_ioctl = my_ioctl,
};
 
static int __init basic_init(void)
{
    int ret;
 
    // Register character device with dynamic major number
    major_num = register_chrdev(MAJOR_NUM, DEVICE_NAME, &fops);
    if (major_num < 0) {
        pr_err("Failed to register char device: %d\n", major_num);
        return major_num;
    }
 
    // Get PWM device - try different possible names
    pwm0 = pwm_get(NULL, "pwm0");
    if (IS_ERR(pwm0)) {
        pwm0 = pwm_get(NULL, "0");  // Try alternative name
        if (IS_ERR(pwm0)) {
            pr_err("Failed to get PWM device: %ld\n", PTR_ERR(pwm0));
            ret = PTR_ERR(pwm0);
            goto unregister_chrdev;
        }
    }
 
    // Configure initial PWM
    ret = pwm_config(pwm0, period_ns * duty_cycle / 100, period_ns);
    if (ret) {
        pr_err("Failed to configure PWM: %d\n", ret);
        goto put_pwm;
    }
 
    ret = pwm_enable(pwm0);
    if (ret) {
        pr_err("Failed to enable PWM: %d\n", ret);
        goto put_pwm;
    }
 
    pr_info("%s: Module loaded, Major %d, PWM initialized at %u%% duty cycle\n", 
            DEVICE_NAME, major_num, duty_cycle);
    return 0;
 
put_pwm:
    pwm_put(pwm0);
unregister_chrdev:
    unregister_chrdev(major_num, DEVICE_NAME);
    return ret;
}
 
static void __exit basic_exit(void)
{
    if (!IS_ERR(pwm0)) {
        pwm_disable(pwm0);
        pwm_put(pwm0);
        pr_info("PWM disabled and released\n");
    }
    
    unregister_chrdev(major_num, DEVICE_NAME);
    pr_info("Module unloaded\n");
}
 
module_init(basic_init);
module_exit(basic_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("TECHDHBA");
MODULE_DESCRIPTION("PWM LED driver with IOCTL control");
MODULE_VERSION("1.0");
 
