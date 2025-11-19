#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>

static int __init my_init(void){
	pr_info("hello world\n");
	return 0;
}

static void __exit my_exit(void){
	pr_info("exiting\n");

}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("devashree katarkar");
MODULE_DESCRIPTION("basic driver registration");
