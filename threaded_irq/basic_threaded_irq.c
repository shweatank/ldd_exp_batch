// basic_threaded_irq.c
// Super simple threaded IRQ example

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/moduleparam.h>
#include <linux/delay.h>

MODULE_LICENSE("GPL");

/* Pass IRQ number while inserting */
static int irq = 1;
module_param(irq, int, 0444);
MODULE_PARM_DESC(irq, "IRQ number");

static char *name = "basic_threaded_irq";
module_param(name, charp, 0444);

/*
 * TOP HALF (HARD IRQ)
 * Runs in interrupt context, CANNOT sleep.
 * Do very fast work, and return IRQ_WAKE_THREAD.
 */
static irqreturn_t irq_top(int irq, void *data)
{
    pr_info("%s: TOP HALF (hard IRQ) fired irq=%d\n", name, irq);

    /* Minimal work only */
    /* No sleeping, no blocking here */

    /* Tell kernel: run the threaded handler */
    return IRQ_WAKE_THREAD;
}

/*
 * THREADED HANDLER (BOTTOM HALF)
 * Runs in kthread context -> CAN SLEEP
 */
static irqreturn_t irq_thread(int irq, void *data)
{
    pr_info("%s: THREADED handler start (irq=%d)\n", name, irq);

    /* Simulate some processing */
    msleep(100);   // allowed here

    pr_info("%s: THREADED handler end\n", name);

    return IRQ_HANDLED;
}

/*
 * Module init - request IRQ
 */
static int __init basic_threaded_irq_init(void)
{
    int ret;

    pr_info("%s: loading… requesting IRQ=%d\n", name, irq);

    /* Request a threaded IRQ */
    ret = request_threaded_irq(
            irq,
            irq_top,        /* top half */
            irq_thread,     /* bottom half( thread ) */
            0,              /* no special flags */
            name,
            NULL
    );

    if (ret) {
        pr_err("%s: request_threaded_irq failed %d\n", name, ret);
        return ret;
    }

    pr_info("%s: successfully registered IRQ %d\n", name, irq);
    return 0;
}

/*
 * Module exit - cleanup
 */
static void __exit basic_threaded_irq_exit(void)
{
    free_irq(irq, NULL);
    pr_info("%s: unloaded driver, freed IRQ %d\n", name, irq);
}

module_init(basic_threaded_irq_init);
module_exit(basic_threaded_irq_exit);
