#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/delay.h>

static struct task_struct *task1;
static struct task_struct *task2;

static int counter = 0;   // shared variable → race condition

static int thread_fn(void *arg)
{
    int id = *(int *)arg;

    pr_info("Thread %d starting (pid=%d)\n", id, current->pid);

    while (!kthread_should_stop()) {

        /* --- RACE CONDITION HERE --- */
        counter++;  
        pr_info("Thread %d: counter = %d\n", id, counter);

        ssleep(1);
    }

    pr_info("Thread %d stopping\n", id);
    return 0;
}

static int __init race_init(void)
{
    static int id1 = 1, id2 = 2;

    pr_info("Race condition demo init\n");

    task1 = kthread_run(thread_fn, &id1, "race_thread1");
    task2 = kthread_run(thread_fn, &id2, "race_thread2");

    if (IS_ERR(task1) || IS_ERR(task2)) {
        pr_err("Failed to create threads\n");
        return -1;
    }

    return 0;
}

static void __exit race_exit(void)
{
    pr_info("Stopping threads...\n");

    if (task1)
        kthread_stop(task1);

    if (task2)
        kthread_stop(task2);

    pr_info("Final counter = %d\n", counter);
}

module_init(race_init);
module_exit(race_exit);

MODULE_LICENSE("GPL");

