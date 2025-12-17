#include<linux/init.h>
#include<linux/module.h>

MODULE_LICENSE("GPL");

int shared_value = 42;

EXPORT_SYMBOL(shared_value); //used in Linux kernel modules to make a variable or function available to other loadable modules.

static int __init start(void){
	pr_info("driver loaded shared value %d\n",shared_value);
	return 0;
}

static void __exit end(void){
	pr_info("driver unloaded\n");
}

module_init(start);
module_exit(end);
