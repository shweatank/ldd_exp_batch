#include <linux/module.h>
#include <linux/workqueue.h>

static void work_handler(struct work_struct *work);
static void delayed_work_handler(struct work_struct *work);

static DECLARE_WORK(my_work, work_handler);
static DECLARE_DELAYED_WORK(my_delayed_work, delayed_work_handler);

static void work_handler(struct work_struct *work)
{
    pr_info("Work handler executed\n");
}

static void delayed_work_handler(struct work_struct *work)
{
    pr_info("Delayed work handler executed\n");
}

static int __init my_init(void)
{
    schedule_work(&my_work);
    schedule_delayed_work(&my_delayed_work, msecs_to_jiffies(2000));
    return 0;
}

static void __exit my_exit(void)
{
    cancel_work_sync(&my_work);
    cancel_delayed_work_sync(&my_delayed_work);
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
