/* ---- memleak_ex.c ---- */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Kmemleak unreachable memory example (memory leak)");

static int *leaked_ptr;
static int __init kmemleak_unreachable_init(void)
{

    printk(KERN_INFO "Kmemleak unreachable example: Initializing\n");

    // Allocate memory
    leaked_ptr = kmalloc(sizeof(int), GFP_KERNEL);
    if (!leaked_ptr) {
        printk(KERN_ERR "Failed to allocate memory\n");
        return -ENOMEM;
    }

    printk(KERN_INFO "Memory allocated at %p\n",leaked_ptr);
    // Intentionally lose the pointer (memory leak!)
    // By assigning a new value to leaked_ptr, the original allocated
    // memory becomes unreachable.
    leaked_ptr = NULL;


    return 0;
}

static void __exit kmemleak_unreachable_exit(void)
{
    printk(KERN_INFO "Kmemleak unreachable example: Exiting\n");
    // No kfree here!  The memory is leaked.
}

module_init(kmemleak_unreachable_init);
module_exit(kmemleak_unreachable_exit);

