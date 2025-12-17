#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/sched/signal.h>
#include <linux/errno.h>

static struct task_struct *demo_task;
static int counter;

static int demo_fn(void *arg)
{
    int *cnt = (int *)arg;

    pr_info("TechDhaba: demo_fn started (pid=%d, name=%s)\n",
            current->pid, current->comm);

    while (!kthread_should_stop())
    {
        (*cnt)++;
        pr_info("TechDhaba: tick = %d\n", *cnt);
        msleep(1000);    // sleep 1 second
    }

    pr_info("TechDhaba: demo_fn stopping (final tick = %d)\n", *cnt);

    return 0;
}

static int __init techdhaba_kthread_init(void)
{
    pr_info("TechDhaba: kthread demo init\n");

    counter = 0;

    demo_task = kthread_run(demo_fn, &counter, "techdhaba_demo");
    if (IS_ERR(demo_task))
    {
        pr_err("TechDhaba: failed to create kthread (%ld)\n",
               PTR_ERR(demo_task));
        return PTR_ERR(demo_task);
    }

    pr_info("TechDhaba: kthread spawned (pid=%d, name=%s)\n",
            demo_task->pid, demo_task->comm);

    return 0;
}

static void __exit techdhaba_kthread_exit(void)
{
    int ret;

    pr_info("TechDhaba: kthread demo exit\n");

    if (demo_task)
    {
        ret = kthread_stop(demo_task);
        pr_info("TechDhaba: kthread stopped, thread returned %d\n", ret);
        demo_task = NULL;
    }
}

module_init(techdhaba_kthread_init);
module_exit(techdhaba_kthread_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("TechDhaba");
MODULE_DESCRIPTION("Kernel Thread Program Example");

