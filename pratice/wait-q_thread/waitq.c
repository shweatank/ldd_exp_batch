#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/kthread.h>
#include<linux/wait.h>
#include<linux/delay.h>
#include<linux/timer.h>
static struct task_struct *c_thread;
static int condition =0;
static struct timer_list my_time;
static wait_queue_head_t my_wq;
static void timer_callback_fun(struct timer_list *t){
	pr_info("waking thread\n");
	condition=1;
	wake_up(&my_wq); //waiting thread wake up
	
	mod_timer(&my_time, jiffies+msecs_to_jiffies(3000)); //after 3 second
}

static int consumer_fn(void *data){
	while(!kthread_should_stop()){
		pr_info("waiting for condition\n");
		wait_event(my_wq, condition == 1);
		pr_info("process start\n");
		condition = 0;
		ssleep(1);
	}
	return 0;
}

static int __init start(void){
	pr_info("waitwueue loaded\n");
	init_waitqueue_head(&my_wq);
	c_thread=kthread_run(consumer_fn, NULL, "consumer_thread");

	timer_setup(&my_time, timer_callback_fun, 0);
	mod_timer(&my_time, jiffies+msecs_to_jiffies(3000));

	return 0;
}

static void __exit end(void){

	pr_info("wait queue remove\n");
	kthread_stop(c_thread);
	del_timer_sync(&my_time);
}

module_init(start);
module_exit(end);
MODULE_LICENSE("GPL");
