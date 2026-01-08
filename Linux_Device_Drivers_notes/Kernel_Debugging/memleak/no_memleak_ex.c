/* ---- no_memleak_ex.c ---- */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Kmemleak reachable memory example");

static int *ptr;

static int __init kmemleak_reachable_init(void)
{
    printk(KERN_INFO "Kmemleak reachable example: Initializing\n");

    // Allocate memory
    ptr = kmalloc(sizeof(int), GFP_KERNEL);
    if (!ptr) {
        printk(KERN_ERR "Failed to allocate memory\n");
        return -ENOMEM;
    }

    // Initialize the allocated memory
    *ptr = 123;

    printk(KERN_INFO "Allocated memory at: %p, value: %d\n", ptr, *ptr);

    return 0;
}

static void __exit kmemleak_reachable_exit(void)
{
    printk(KERN_INFO "Kmemleak reachable example: Exiting\n");

    // Free the allocated memory
    kfree(ptr);
    printk(KERN_INFO "Freed memory at: %p\n", ptr);
}

module_init(kmemleak_reachable_init);
module_exit(kmemleak_reachable_exit);

