#include<linux/init.h>
#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/kthread.h>
#include<linux/delay.h>
#include<linux/sched/signal.h>
#include<linux/errno.h>


static struct task_struct *demo_task;

static int count;


static int demo_fn(void *arg)
{
	int *cnt=(int *)arg;

	pr_info("demo_fn starting (pid=%d, name=%s)\n",current->pid, current->comm);

	while(!kthread_should_stop())
	{
		(*cnt)++;
		pr_info("demo_fn tick=%d (pid=%d)\n",*cnt,current->pid);

		ssleep(1);
	}
	pr_info("demo_fn stopping (final tick=%d)\n",*cnt);
	return 0;
}

static int __init hi(void)
{
	pr_info("init function is invoked\n");
	count=0;

	demo_task=kthread_run(demo_fn,&count,"saivarshath");

	if(IS_ERR(demo_task))
	{
		pr_err("failed to create a thread (%d)\n",PTR_ERR(demo_task));
		return PTR_ERR(demo_task);
	}
	pr_info("spawned (pid=%d, name=%s)\n",demo_task->pid,demo_task->comm);

	return 0;
}

static void __exit hello(void)
{
	int ret=0;

	pr_info("Thread demo exit\n");

	if(demo_task)
	{
		ret=kthread_stop(demo_task);
		pr_info("kthread stopped, threadfn returned %d\n",ret);
		demo_task=NULL;
	}
}

module_init(hi);
module_exit(hello);

MODULE_LICENSE("GPL");

