#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/sched/task.h>
#include <linux/sched/signal.h>   // for_each_child_of_task(), thread iteration
#include <linux/cpumask.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Prashant");
MODULE_DESCRIPTION("Print detailed process info");
MODULE_VERSION("1.0");

static int __init process_info_init(void)
{
    struct task_struct *task = current;
    struct task_struct *child;
    struct task_struct *thread;

    pr_info("=== Process Info Module Loaded ===\n");

    // 1. Current PID
    pr_info("Current: %s | PID = %d\n", task->comm, task->pid);

    // 2. Parent PID
    if (task->parent)
        pr_info("Parent: %s | PPID = %d\n", task->parent->comm, task->parent->pid);

    // 3. CPU ID this task is executing on
    pr_info("Running on CPU: %d\n", task_cpu(task));

    // 4. All child PIDs
    pr_info("Children of %s (PID %d):\n", task->comm, task->pid);

    rcu_read_lock();
    list_for_each_entry(child, &task->children, sibling) {
        pr_info("  Child -> %s | PID = %d\n", child->comm, child->pid);
    }
    rcu_read_unlock();

    // 5. All thread group PIDs (threads of same process)
    pr_info("Thread Group (TGID = %d):\n", task->tgid);

    rcu_read_lock();
    for_each_thread(task, thread) {
        pr_info("  Thread -> %s | TID = %d\n", thread->comm, thread->pid);
    }
    rcu_read_unlock();

    pr_info("==================================\n");

    return 0;
}

static void __exit process_info_exit(void)
{
    pr_info("Process info module unloaded.\n");
}

module_init(process_info_init);
module_exit(process_info_exit);
