// driver_a.c
#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Prashant");
MODULE_DESCRIPTION("Driver A exporting a variable");

int shared_value = 42;        // The external variable

EXPORT_SYMBOL(shared_value);  // Export it for other modules

static int __init driver_a_init(void)
{
    pr_info("Driver A loaded. shared_value=%d\n", shared_value);
    return 0;
}

static void __exit driver_a_exit(void)
{
    pr_info("Driver A unloaded.\n");
}

module_init(driver_a_init);
module_exit(driver_a_exit);
