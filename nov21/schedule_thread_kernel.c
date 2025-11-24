#include <linux/kthread.h>
#include <linux/sched.h>
#include <linux/sched/signal.h>

static struct task_struct *my_thread;

static int thread_fn(void *data)
{
    while (!kthread_should_stop()) {
        pr_info("RT kthread running...\n");
        ssleep(1);
    }
    return 0;
}

static int __init mymod_init(void)
{
    struct sched_param param = { .sched_priority = 50 }; // range: 1–99

    my_thread = kthread_run(thread_fn, NULL, "rt_kthread");
    if (IS_ERR(my_thread))
        return PTR_ERR(my_thread);

    // Set real-time priority
    sched_setscheduler(my_thread, SCHED_FIFO, &param);

    pr_info("kthread priority set to FIFO:50\n");
    return 0;
}

static void __exit mymod_exit(void)
{
    kthread_stop(my_thread);
}

module_init(mymod_init);
module_exit(mymod_exit);
MODULE_LICENSE("GPL");
