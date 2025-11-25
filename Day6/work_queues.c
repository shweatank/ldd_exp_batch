#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/workqueue.h>
#include <linux/delay.h>

/* Forward declarations of work handler functions */
static void work_handler(struct work_struct *work);
static void delayed_work_handler(struct work_struct *work);

/* Pointer to custom workqueue */
static struct workqueue_struct *my_wq;

/* Declare normal and delayed work items */
static DECLARE_WORK(my_work, work_handler);
static DECLARE_DELAYED_WORK(my_delayed_work, delayed_work_handler);

/* Normal work handler */
static void work_handler(struct work_struct *work)
{
    pr_info("Workqueue: Normal work executed\n");
}

/* Delayed work handler */
static void delayed_work_handler(struct work_struct *work)
{
    pr_info("Workqueue: Delayed work executed after 2 seconds\n");
}

/* Module init function */
static int __init mymod_init(void)
{
    pr_info("Workqueue example loaded\n");

    /* Create a custom workqueue */
    my_wq = alloc_workqueue("my_wq", WQ_UNBOUND | WQ_HIGHPRI, 0);
    if (!my_wq) {
        pr_err("Failed to create workqueue\n");
        return -ENOMEM;
    }

    /* Queue normal work */
    queue_work(my_wq, &my_work);

    /* Queue delayed work (2 seconds) */
    queue_delayed_work(my_wq, &my_delayed_work, msecs_to_jiffies(2000));

    return 0;
}

/* Module exit function */
static void __exit mymod_exit(void)
{
    pr_info("Workqueue example unloaded\n");

    /* Cancel delayed work and flush workqueue */
    cancel_delayed_work_sync(&my_delayed_work);
    flush_workqueue(my_wq);

    /* Destroy workqueue */
    destroy_workqueue(my_wq);
}

module_init(mymod_init);
module_exit(mymod_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Varshath");
MODULE_DESCRIPTION("Linux Kernel Module: Workqueue Example");

