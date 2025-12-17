#include <linux/kernel.h>
#include <linux/gpio.h>
#include <linux/timer.h>
#include <linux/module.h>
#include <linux/init.h>
#include"header.h"

int major;
#define DEVICE_NAME "header_file"
static int __init print_init(void){
	major= register_chrdev(0, DEVICENE_NAME);
	printk("major number %d \n x : %d\n", major,x);
	return 0;
}

static void __exit print_exit(void){
	unregister_chrdev(major, DEVICE_NAME);
	printk("\ndevice unloaded\n");
}

module_init(print_init);
module_exit(print_exit);
MODULE_LICENSE("GPL");
