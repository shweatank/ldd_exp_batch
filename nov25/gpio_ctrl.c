#include <linux/kernel.h>
#include <linux/gpio.h>
#include <linux/fs.h>
#include <linux/ioctl.h>
#include <linux/module.h>
#include <linux/init.h>

#define DEVICE_NAME "ioctl_led_gpio"
#define MAJOR_NUM 		100
#define LED_PIN 529
#define IOCTL_SET_LED _IOW(MAJOR_NUM, 0, int)
#define IOCTL_CLEAR_LED _IOR(MAJOR_NUM, 1, int)

static int led=0;

static long my_ioctl(struct file *file, unsigned int cmd, unsigned long arg);
static int my_open(struct inode *inode, struct file *file) {
     pr_info("ioctl demo: Device opened\n");
     return 0;
}

static int my_release(struct inode *inode, struct file *file) {
	pr_info("ioctl demo: Device closed\n");
 	return 0;
}

static long my_ioctl(struct file *file, unsigned int cmd, unsigned long arg) {
	switch(cmd) {
		case IOCTL_SET_LED:
			led = 1;
			gpio_set_value(LED_PIN, led);
			printk(KERN_INFO "Led = %d\n", led);
			break;
		case IOCTL_CLEAR_LED:
			led = 0;
			gpio_set_value(LED_PIN, led);
			printk(KERN_INFO "Led = %d\n", led);
			break;
		default:
			printk(KERN_INFO "Invalid command");

	}
	return 0;
}

struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = my_open,
	.release = my_release,
	.unlocked_ioctl = my_ioctl,
};

static int __init basic_init(void) {
	int ret;
	ret = register_chrdev(MAJOR_NUM, DEVICE_NAME, &fops);
	if (ret < 0) {
         pr_err("Failed to regiter char dev\n");
         return ret;
     }

	if (gpio_is_valid(LED_PIN) == false) {
		printk(KERN_INFO "the gpio pin is not valid\n");
		return -1;
	}
	ret = gpio_request(LED_PIN, "LED");
	if (ret<0) {
		printk(KERN_INFO "the gpio request fail\n");
		return ret;
	}
	gpio_direction_output(LED_PIN,0);


	printk(KERN_INFO "init module loaded\n");
	pr_info("ioctl demo: Module loaded, Major %d\n", MAJOR_NUM);

	return 0;
}



static void __exit basic_exit(void)
{
	unregister_chrdev(MAJOR_NUM, DEVICE_NAME);
	gpio_free(LED_PIN);
	printk(KERN_INFO"module was unloaded\n");
}


module_init(basic_init);
module_exit(basic_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("TECHDHBA");
MODULE_DESCRIPTION("BASIC GPIO DRIVER - LED_BLINK");











