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
    pr_info("TechDhaba: demo_fn starting (pid = %d, name =%s)\n",current->pid, current->comm);
    
    while(!kthread_should_stop()) {
           (*cnt)++;
    pr_info("Techdhaba : demo_fn tick = %d (pid = %d)\n",*cnt, current->pid);


   ssleep(1);
}

pr_info("Techdhaba : demo_fn stopping final tick + %d\n",*cnt);
 return 0;
}

static int __init  techdhaba_kthread_init(void)
{
      pr_info("Techdhaba : kthread demo init\n");
       counter = 0;


     demo_task = kthread_run(demo_fn, &counter, "Techdhaba_demo");
     if(IS_ERR(demo_task)){
          pr_err("Techdhaba: faile to create kthread (%ld)\n",PTR_ERR(demo_task));
         return PTR_ERR(demo_task);
}

    pr_info("Techdhaba : kthread spawned (pid = %d,name = %s)\n",demo_task->pid,demo_task->comm);

    return 0;
}


static void __exit techdhaba_kthread_exit(void)
{
    int ret = 0;
    pr_info("Techdhaba : kthread demo exit\n");
     if(demo_task){
           ret = kthread_stop(demo_task);
           pr_info("Techdhaba : kthread stopped, threadfn returned %d \n",ret);
           demo_task = NULL;
         }
}

module_init(techdhaba_kthread_init);
module_exit(techdhaba_kthread_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("techdhaba");
MODULE_DESCRIPTION("kthread");
















