#include <linux/module.h>

extern int base_func(void);

static int __init drv_init(void)
{
    printk("value=%d\n", base_func());
    return 0;
}

static void __exit drv_exit(void) {}

module_init(drv_init);
module_exit(drv_exit);
MODULE_LICENSE("GPL");
