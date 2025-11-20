#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/sched/signal.h>
#include <linux/errno.h>

static struct task_struct *demo_task;

static int counter;


static int demo_fn(void *arg){
	int *cnt = (int *)arg;

	pr_info("metadata: pid=%d name=%s",current->pid, current->comm);

	while(!kthread_should_stop()){
		(*cnt)++;
		pr_info("tick=%d pid=%d\n",*cnt, current->pid);

		ssleep(1);
	}

	pr_info("final tick=%d\n",*cnt);
	return 0;
}

static int __init my_init(void){
	
	pr_info("thread init\n");

	counter = 0;

	demo_task = kthread_run(demo_fn, &counter, "demo");

	if(IS_ERR(demo_task)){
		pr_err("failed to create kthread %ld\n", PTR_ERR(demo_task));
		return PTR_ERR(demo_task);
	}

	pr_info("kthread spwaned: pid=%d, name=%s\n",demo_task->pid, demo_task->comm);
	return 0;

}

static void __exit my_exit(void){
	int ret = 0;

	pr_info("exit called");

	if(demo_task){
		ret = kthread_stop(demo_task);
		pr_info("thredfun ret: %d\n",ret);
		demo_task = NULL;
	}

}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Devashree katarkar");
MODULE_DESCRIPTION("A sample basic driver");
