// driver_b.c
#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Prashant");
MODULE_DESCRIPTION("Driver B using external variable");

extern int shared_value;   // Declare external variable

static int __init driver_b_init(void)
{
    pr_info("Driver B loaded. External shared_value = %d\n", shared_value);

    // Modify it
    shared_value += 10;
    pr_info("Driver B updated shared_value = %d\n", shared_value);

    return 0;
}

static void __exit driver_b_exit(void)
{
    pr_info("Driver B unloaded.\n");
}

module_init(driver_b_init);
module_exit(driver_b_exit);
