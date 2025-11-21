#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/kthread.h>
#include <linux/errno.h>
#include <linux/sched/signal.h>
 
static struct task_struct *demo_task;
static int counter;
 
 
static int demo_fn(void *arg) {
	int *cnt = (int *)arg;
 
	pr_info("Demo function started pid = %d and name = %s\n", current->pid, current->comm);
 
	while(!kthread_should_stop()) {
		(*cnt)++;
		pr_info("Demo function tick=%d pid=%d\n", *cnt, current->pid);
 
		ssleep(1);
	}
 
	pr_info("Demo function stopped final tick = %d\n", *cnt);
	return 0;
 
}
 
static int __init yawar_kthread_init(void) {
	pr_info("Kthread demo init\n");
	counter = 0;
	demo_task = kthread_run(demo_fn, &counter, "Kthread_Demo");
 
	if (IS_ERR(demo_task)) {
		pr_err("Failed to create thread %ld\n", PTR_ERR(demo_task));
		return PTR_ERR(demo_task);
        
	pr_info("kthread spawned pid = %d name = %s\n", demo_task->pid, demo_task->comm);
	}

    return 0;
}

static void __exit yawar_kthread_exit(void){
    int ret = 0;
    pr_info("kthread demo exit\n");

    if (demo_task)
    {
        ret = kthread_stop(demo_task);
        pr_info("kthread stopped returned %d\n",ret);
        demo_task = NULL;
    }
}

module_init(yawar_kthread_init);
module_exit(yawar_kthread_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("YAWAR");
MODULE_DESCRIPTION("kthread interrupt handler.");

