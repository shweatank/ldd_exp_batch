// SPDX-License-Identifier: GPL-2.0
/*
 * irq_tasklet_demo.c
 *
 * Simple example where an interrupt handler (top half)
 * defers heavier work to a tasklet (bottom half).
 *
 *  - You pass an IRQ number as module parameter: irq=N
 *  - The IRQ handler just bumps a counter and schedules a tasklet.
 *  - The tasklet runs later in softirq context and prints a message.
 *
 *  WARNING: Only use on a test machine and a known IRQ line.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/init.h>
#include<linux/delay.h>
static int irq = -1;                  /* IRQ to hook */
module_param(irq, int, 0444);
MODULE_PARM_DESC(irq, "IRQ number to attach to");

static char *devname = "irq_tasklet_demo";
module_param(devname, charp, 0444);
MODULE_PARM_DESC(devname, "dev_id name for shared IRQ");

static unsigned long irq_count;
static unsigned long tasklet_count;

/* Our tasklet structure */
static struct tasklet_struct demo_tasklet;

/* ---------- Bottom half: tasklet ---------- */

static void demo_tasklet_func(unsigned long data)
{

	tasklet_count++;
	int h=100;
	while(h--)
	{
	pr_info("irq_tasklet_demo: tasklet running, data=%lu, tasklet_count=%lu (irq_count=%lu)\n",
		data, tasklet_count, irq_count);
	ssleep(1);
	}
}

/* ---------- Top half: IRQ handler ---------- */

static irqreturn_t demo_irq_handler(int irq_num, void *dev_id)
{
	irq_count++;

	/*
	 * Keep top half as small as possible:
	 * just schedule the tasklet and return.
	 */
	tasklet_schedule(&demo_tasklet);

	/* If this IRQ is shared, we "claim" it unconditionally.
	 * On real drivers you must check device-specific status
	 * and return IRQ_NONE when it's not your interrupt.
	 */
	return IRQ_HANDLED;
}

/* ---------- Module init / exit ---------- */

static int __init irq_tasklet_demo_init(void)
{
	int ret;
	unsigned long tasklet_data = 1234;

	if (irq < 0) {
		pr_err("irq_tasklet_demo: please specify a valid irq=N\n");
		return -EINVAL;
	}

	pr_info("irq_tasklet_demo: loading, irq=%d, devname=%s\n", irq, devname);

	/* Init tasklet: bottom half */
	tasklet_init(&demo_tasklet, demo_tasklet_func, tasklet_data);

	/* Request IRQ (shared so we don't steal it exclusively) */
	ret = request_irq(irq,
			  demo_irq_handler,
			  IRQF_SHARED,
			  devname,
			  (void *)devname);
	if (ret) {
		pr_err("irq_tasklet_demo: request_irq(%d) failed: %d\n", irq, ret);
		tasklet_kill(&demo_tasklet);
		return ret;
	}

	pr_info("irq_tasklet_demo: successfully registered on IRQ %d\n", irq);
	return 0;
}

static void __exit irq_tasklet_demo_exit(void)
{
	pr_info("irq_tasklet_demo: unloading, irq=%d\n", irq);

	/* Free IRQ */
	if (irq >= 0)
		free_irq(irq, (void *)devname);

	/* Ensure tasklet is not running anymore */
	tasklet_kill(&demo_tasklet);

	pr_info("irq_tasklet_demo: final counts: irq=%lu, tasklet=%lu\n",
		irq_count, tasklet_count);
}

module_init(irq_tasklet_demo_init);
module_exit(irq_tasklet_demo_exit);

MODULE_AUTHOR("TechDhaba demo");
MODULE_DESCRIPTION("Example: tasklet triggered from IRQ upper half");
MODULE_LICENSE("GPL");
