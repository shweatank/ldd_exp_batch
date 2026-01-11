#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A simple module to generate a kernel OOPS");

static int __init oops_example_init(void)
{
    int *ptr = NULL;

    printk(KERN_INFO "Oops Example Module: Initializing\n");

    // Deliberately dereference a NULL pointer
    printk(KERN_INFO "Value at ptr: %d\n", *ptr);  // This will cause an OOPS! 

    return 0; // Will not reach here
}

static void __exit oops_example_exit(void)
{
    printk(KERN_INFO "Oops Example Module: Exiting\n");
}

module_init(oops_example_init);
module_exit(oops_example_exit);

