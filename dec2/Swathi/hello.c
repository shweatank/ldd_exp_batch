#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

static int __init my_init(void) {
	pr_info("Init called\n");
	pr_info("Hello world\n");
	return 0;
}

static void __exit my_exit(void) {
	pr_info("Exit called\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Swathi");
MODULE_DESCRIPTION("Sample hello world\n");
