// timer_normal.c
// Simple periodic kernel timer using struct timer_list
// Also demonstrates deferring work to a workqueue from the timer callback.
//
// Build with provided Makefile and test on a disposable machine/VM.

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/timer.h>
#include <linux/jiffies.h>
#include <linux/workqueue.h>
#include <linux/param.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("TechDhaba");
MODULE_DESCRIPTION("Normal kernel timer (timer_list) example with workqueue");
MODULE_VERSION("1.0");

/* module parameter: period in milliseconds */
static unsigned int period_ms = 1000;
module_param(period_ms, uint, 0644);
MODULE_PARM_DESC(period_ms, "Timer period in milliseconds");

static struct timer_list my_timer;
static atomic64_t timer_count;

/* Work item used to do heavier processing outside timer context */
static struct workqueue_struct *my_wq;
static DECLARE_WORK(my_work, NULL); /* will set function later */

/* Work handler (runs in process context) */
static void my_work_handler(struct work_struct *work)
{
	long long count = atomic64_read(&timer_count);
	pr_info("timer_normal: workqueue handler executing (callback_count=%lld)\n", count);

	/* Place heavier processing here (safe to sleep if needed) */
}

/* Timer callback (runs in softirq context) */
static void my_timer_callback(struct timer_list *t)
{
	/* increment callback counter */
	atomic64_inc(&timer_count);

	pr_info("timer_normal: timer fired (count=%lld) — scheduling work\n",
			atomic64_read(&timer_count));

	/* Defer heavy work to workqueue */
	if (my_wq)
		queue_work(my_wq, &my_work);

	/* Re-arm the timer for the next period */
	mod_timer(&my_timer, jiffies + msecs_to_jiffies(period_ms));
}

static int __init timer_module_init(void)
{
	pr_info("timer_normal: init (period=%u ms)\n", period_ms);
	atomic64_set(&timer_count, 0);

	/* create single-threaded workqueue (can be NULL to use system one) */
	my_wq = create_singlethread_workqueue("timer_normal_wq");
	if (!my_wq) {
		pr_err("timer_normal: failed to create workqueue\n");
		return -ENOMEM;
	}

	/* initialize work with handler */
	INIT_WORK(&my_work, my_work_handler);

	/* initialize timer and set callback */
	timer_setup(&my_timer, my_timer_callback, 0);

	/* arm timer for first expiry after period_ms */
	mod_timer(&my_timer, jiffies + msecs_to_jiffies(period_ms));

	return 0;
}

static void __exit timer_module_exit(void)
{
	int ret;

	/* cancel timer and ensure callback finished */
	ret = del_timer_sync(&my_timer);
	if (ret)
		pr_info("timer_normal: timer was active and cancelled\n");

	/* flush and destroy workqueue */
	if (my_wq) {
		flush_workqueue(my_wq);
		destroy_workqueue(my_wq);
		my_wq = NULL;
	}

	pr_info("timer_normal: exit (callbacks=%lld)\n", atomic64_read(&timer_count));
}

module_init(timer_module_init);
module_exit(timer_module_exit);
