#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/workqueue.h>
#include <linux/delay.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Swapnil");
MODULE_DESCRIPTION("Simple Workqueue Demo");

// --------------------------------------------------
// Forward declarations (prototypes)
// --------------------------------------------------
static void my_work_handler(struct work_struct *work);
static void my_delayed_work_handler(struct work_struct *work);

// --------------------------------------------------
// Workqueue objects
// --------------------------------------------------
static DECLARE_WORK(my_work, my_work_handler);
static DECLARE_DELAYED_WORK(my_delayed_work, my_delayed_work_handler);

// --------------------------------------------------
// Workqueue Handlers
// --------------------------------------------------
static void my_work_handler(struct work_struct *work)
{
    pr_info("Normal workqueue executed in process: %s (pid: %d)\n",
            current->comm, current->pid);
}

static void my_delayed_work_handler(struct work_struct *work)
{
    pr_info("Delayed work executed after delay! process: %s (pid: %d)\n",
            current->comm, current->pid);
}

// --------------------------------------------------
// Module Init Function
// --------------------------------------------------
static int __init my_wq_init(void)
{
    pr_info("Workqueue module loaded\n");

    // Schedule immediate work
    pr_info("Scheduling immediate work...\n");
    schedule_work(&my_work);

    // Schedule delayed work (2 seconds)
    pr_info("Scheduling delayed work (2 sec)...\n");
    schedule_delayed_work(&my_delayed_work, 2 * HZ);

    return 0;
}

// --------------------------------------------------
// Module Exit Function
// --------------------------------------------------
static void __exit my_wq_exit(void)
{
    pr_info("Workqueue module unloading...\n");

    cancel_work_sync(&my_work);
    cancel_delayed_work_sync(&my_delayed_work);

    pr_info("Workqueue module unloaded\n");
}

module_init(my_wq_init);
module_exit(my_wq_exit);

