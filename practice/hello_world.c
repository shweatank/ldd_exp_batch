#include <linux/module.h>
#include <linux/init.h>

static int __init hello_world_init(void){
	printk("Helllo world Init\n");
	return 0;

}

static void __exit hello_world_exit(void){
	printk("Hello world Exit\n");
}

module_init(hello_world_init);
module_exit(hello_world_exit);

MODULE_LICENSE("GPL");
