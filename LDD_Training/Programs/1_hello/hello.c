#include <linux/module.h>
#include <linux/init.h>

MODULE_LICENSE("Dual BSD/GPL");

static int hello_init(void) {
	printk(KERN_ALERT "Hello, World!\n");
	return -ENOMEM;
}

static void hello_exit(void) 
{
	printk(KERN_ALERT "Goodbye, World\n");
	return;
}


module_init(hello_init);
module_exit(hello_exit);

