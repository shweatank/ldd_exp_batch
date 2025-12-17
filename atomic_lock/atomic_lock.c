/*
 * sync_atomic_demo.c - Very simple atomic_t example
 *
 * A timer increments an atomic counter.
 * init/exit display its value.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/timer.h>
#include <linux/jiffies.h>
#include <linux/atomic.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("techdhaba");
MODULE_DESCRIPTION("Simple atomic_t synchronization demo");
MODULE_VERSION("1.0");

static atomic_t counter = ATOMIC_INIT(0);
static struct timer_list my_timer;

static void my_timer_func(struct timer_list *t)
{
	int val = atomic_inc_return(&counter);

	pr_info("atomic_demo: timer increment, counter = %d\n", val);

	mod_timer(&my_timer, jiffies + HZ);
}

static int __init sync_atomic_demo_init(void)
{
	pr_info("atomic_demo: init, counter = %d\n",
		atomic_read(&counter));

	timer_setup(&my_timer, my_timer_func, 0);
	mod_timer(&my_timer, jiffies + HZ);

	return 0;
}

static void __exit sync_atomic_demo_exit(void)
{
	del_timer_sync(&my_timer);

	pr_info("atomic_demo: exit, final counter = %d\n",
		atomic_read(&counter));
}

module_init(sync_atomic_demo_init);
module_exit(sync_atomic_demo_exit);
