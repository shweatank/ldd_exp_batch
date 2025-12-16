#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>

#define IR_GPIO_PIN     529
#define DEVICE_NAME     "ir_sensor"
#define CLASS_NAME      "ir_class"

/* IOCTL */
#define IR_MAGIC        'i'
#define IR_GET_STATUS   _IOR(IR_MAGIC, 1, int)

/* Globals */
static int ir_irq;
static int ir_status = 0;

static dev_t dev_num;
static struct cdev ir_cdev;
static struct class *ir_class;
static struct device *ir_device;

/* ---------------- Interrupt Handler ---------------- */
static irqreturn_t ir_irq_handler(int irq, void *dev_id)
{
    ir_status = 1;
    printk(KERN_INFO "IR SENSOR: Object detected\n");
    return IRQ_HANDLED;
}

/* ---------------- File Operations ---------------- */
static int ir_open(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "IR SENSOR: Device opened\n");
    return 0;
}

static int ir_release(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "IR SENSOR: Device closed\n");
    return 0;
}

static long ir_ioctl(struct file *file,
                     unsigned int cmd,
                     unsigned long arg)
{
    int status;

    switch (cmd) {
    case IR_GET_STATUS:
        status = ir_status;
        ir_status = 0;   // clear after read

        if (copy_to_user((int __user *)arg, &status, sizeof(status)))
            return -EFAULT;
        break;

    default:
        return -EINVAL;
    }

    return 0;
}

static struct file_operations ir_fops = {
    .owner          = THIS_MODULE,
    .open           = ir_open,
    .release        = ir_release,
    .unlocked_ioctl = ir_ioctl,
};

/* ---------------- Module Init ---------------- */
static int __init ir_driver_init(void)
{
    int ret;

    printk(KERN_INFO "IR SENSOR DRIVER INIT\n");

    /* Allocate major/minor dynamically */
    ret = alloc_chrdev_region(&dev_num, 0, 1, DEVICE_NAME);
    if (ret)
        return ret;

    /* Initialize and add cdev */
    cdev_init(&ir_cdev, &ir_fops);
    ret = cdev_add(&ir_cdev, dev_num, 1);
    if (ret)
        goto unregister_chrdev;

    /* Create device class */
    ir_class = class_create(CLASS_NAME);
    if (IS_ERR(ir_class)) {
        ret = PTR_ERR(ir_class);
        goto del_cdev;
    }

    /* Create device node */
    ir_device = device_create(ir_class, NULL,
                              dev_num, NULL, DEVICE_NAME);
    if (IS_ERR(ir_device)) {
        ret = PTR_ERR(ir_device);
        goto destroy_class;
    }

    /* GPIO setup */
    gpio_request(IR_GPIO_PIN, "ir_gpio");
    gpio_direction_input(IR_GPIO_PIN);

    /* IRQ setup */
    ir_irq = gpio_to_irq(IR_GPIO_PIN);
    ret = request_irq(ir_irq,
                      ir_irq_handler,
                      IRQF_TRIGGER_FALLING,
                      "ir_irq",
                      NULL);
    if (ret)
        goto destroy_device;

    printk(KERN_INFO "IR sensor driver loaded (major=%d)\n",
           MAJOR(dev_num));
    return 0;

/* Error handling */
destroy_device:
    device_destroy(ir_class, dev_num);
destroy_class:
    class_destroy(ir_class);
del_cdev:
    cdev_del(&ir_cdev);
unregister_chrdev:
    unregister_chrdev_region(dev_num, 1);
    return ret;
}

/* ---------------- Module Exit ---------------- */
static void __exit ir_driver_exit(void)
{
    free_irq(ir_irq, NULL);
    gpio_free(IR_GPIO_PIN);

    device_destroy(ir_class, dev_num);
    class_destroy(ir_class);

    cdev_del(&ir_cdev);
    unregister_chrdev_region(dev_num, 1);

    printk(KERN_INFO "IR SENSOR DRIVER EXIT\n");
}

module_init(ir_driver_init);
module_exit(ir_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("You");
MODULE_DESCRIPTION("IR Sensor GPIO Interrupt Char Driver with Dynamic Device Creation");

