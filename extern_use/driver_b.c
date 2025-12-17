#include<linux/init.h>
#include<linux/module.h>

MODULE_LICENSE("GPL");

extern int shared_value;

static int __init start(void){
	pr_info("B module loaded the shared value %d", shared_value);
	return 0;
}

static void __exit end(void){
	pr_info("B module unloaded\n");
}

module_init(start);
module_exit(end);
