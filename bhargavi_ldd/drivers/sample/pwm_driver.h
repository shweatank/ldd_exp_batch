#ifndef _PWM_DRIVER_H_
#define _PWM_DRIVER_H_

#include <linux/pwm.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "pwm_char"
#define CLASS_NAME  "pwm_class"

#define PWM_PERIOD_NS 20000000   // 20 ms period (50 Hz) - adjust for your hardware

struct pwm_char_dev {
    struct pwm_device *pwm;
    struct pwm_state state;
    dev_t dev_num;
    struct cdev cdev;
    struct class *class;
};

#endif

