// basic_threaded_irq_tasklet.c
// Demonstrates: Top-Half IRQ + Threaded IRQ + Tasklet

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/moduleparam.h>
#include <linux/delay.h>


/* Pass IRQ number at module load */
static int irq = 1;
module_param(irq, int, 0444);
MODULE_PARM_DESC(irq, "IRQ number");

static char *name = "basic_threaded_irq_tasklet";
module_param(name, charp, 0444);

/* ------------------------------------------------------------ */
/* TASKLET FUNCTION (deferred softirq, CANNOT SLEEP)            */
/* ------------------------------------------------------------ */

static void my_tasklet_func(struct tasklet_struct *t)

{
    pr_info("%s: TASKLET running, data=%lu\n", name, data);

    /* No sleeping allowed here */
    /* Do quick actions only */

    pr_info("%s: TASKLET done\n", name);
}

/* Declare tasklet */
//DECLARE_TASKLET(my_tasklet, my_tasklet_func, 1234);
DECLARE_TASKLET(my_tasklet, &my_tasklet_func);

/* ------------------------------------------------------------ */
/* TOP HALF (Hard IRQ) — runs immediately, cannot sleep         */
/* ------------------------------------------------------------ */

static irqreturn_t irq_top(int irq, void *dev)
{
    pr_info("%s: TOP HALF fired (irq=%d)\n", name, irq);

    /* Minimal job only here */

    /* tell kernel to invoke the threaded handler */
    return IRQ_WAKE_THREAD;
}

/* ------------------------------------------------------------ */
/* THREADED HANDLER — can sleep                                */
/* ------------------------------------------------------------ */

static irqreturn_t irq_thread(int irq, void *dev)
{
    pr_info("%s: THREADED handler start (irq=%d)\n", name, irq);

    /* Simulate slow work (allowed) */
    msleep(100);

    pr_info("%s: Scheduling TASKLET from threaded handler\n", name);

    /* schedule tasklet to run later */
    tasklet_schedule(&my_tasklet);
    do_softirq();

    pr_info("%s: THREADED handler end\n", name);

    return IRQ_HANDLED;
}

/* ------------------------------------------------------------ */
/* MODULE INIT                                                  */
/* ------------------------------------------------------------ */

static int __init basic_irq_tasklet_init(void)
{
    int ret;

    pr_info("%s: loading… requesting IRQ=%d\n", name, irq);

    ret = request_threaded_irq(
            irq,
            irq_top,       // top-half
            irq_thread,    // threaded bottom half
            IRQF_SHARED,
            name,
            irq_top
    );

    if (ret) {
        pr_err("%s: request_threaded_irq failed %d\n", name, ret);
        return ret;
    }

    pr_info("%s: registered IRQ %d successfully\n", name, irq);
    return 0;
}

/* ------------------------------------------------------------ */
/* MODULE EXIT                                                  */
/* ------------------------------------------------------------ */

static void __exit basic_irq_tasklet_exit(void)
{
    free_irq(irq, irq_top);

    /* kill tasklet to avoid running after unload */
    tasklet_kill(&my_tasklet);

    pr_info("%s: module unloaded, IRQ %d freed\n", name, irq);
}

module_init(basic_irq_tasklet_init);
module_exit(basic_irq_tasklet_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Devashree katarkar");
MODULE_DESCRIPTION("Simple device driver to implement irq->tasklet->threads");
