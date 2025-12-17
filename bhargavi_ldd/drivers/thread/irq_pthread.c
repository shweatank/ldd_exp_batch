// SPDX-License-Identifier: GPL-2.0
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/wait.h>

static int irq = 1; // Fake IRQ number from module parameter
module_param(irq, int, 0);

MODULE_PARM_DESC(irq, "IRQ number to handle fake packet interrupts");

static unsigned int packet_id = 0;

// Queue structure
struct pkt_data {
    unsigned int id;
    struct list_head list;
};

// Queue head
static LIST_HEAD(pkt_queue);

// Lock for protecting queue
static spinlock_t q_lock;

// Kthread details
static struct task_struct *worker_thread;
static DECLARE_WAIT_QUEUE_HEAD(pkt_waitq);

// Tasklet prototype
static void packet_tasklet_func(unsigned long data);

// Register tasklet
DECLARE_TASKLET(packet_tasklet, packet_tasklet_func, 0);

/*************** IRQ HANDLER (Top Half) *****************/
static irqreturn_t irq_handler(int irq, void *dev)
{
    packet_id++; // simulate receiving packet ID
    pr_info("IRQ: New packet received: %u\n", packet_id);

    tasklet_schedule(&packet_tasklet);
    return IRQ_HANDLED;
}

/*************** TASKLET (Bottom Half) *****************/
static void packet_tasklet_func(unsigned long data)
{
    struct pkt_data *pkt;

    pkt = kmalloc(sizeof(*pkt), GFP_ATOMIC);
    if (!pkt) {
        pr_err("Tasklet: Memory allocation failed!\n");
        return;
    }

    pkt->id = packet_id;

    spin_lock(&q_lock);
    list_add_tail(&pkt->list, &pkt_queue);
    spin_unlock(&q_lock);

    pr_info("Tasklet: Packet ID %u queued for thread\n", pkt->id);

    wake_up_process(worker_thread); // wake kthread
}

/*************** KTHREAD: Heavy processing *****************/
static int kthread_func(void *data)
{
    struct pkt_data *pkt, *tmp;

    while (!kthread_should_stop()) {
        set_current_state(TASK_INTERRUPTIBLE);
        schedule();  // Wait until tasklet wakes us

        if (kthread_should_stop())
            break;

        spin_lock(&q_lock);
        list_for_each_entry_safe(pkt, tmp, &pkt_queue, list) {
            list_del(&pkt->list);
            spin_unlock(&q_lock);

            pr_info("Kthread: Processing packet %u\n", pkt->id);
            msleep(150); // simulate heavy work
            kfree(pkt);

            spin_lock(&q_lock);
        }
        spin_unlock(&q_lock);
    }

    return 0;
}

/*************** MODULE INIT & EXIT *****************/
static int __init irq_tasklet_kthread_init(void)
{
    int ret;

    spin_lock_init(&q_lock);

    worker_thread = kthread_run(kthread_func, NULL, "pkt_worker");
    if (IS_ERR(worker_thread)) {
        pr_err("Failed to create kthread\n");
        return PTR_ERR(worker_thread);
    }

    ret = request_irq(irq, irq_handler, IRQF_SHARED,
                      "irq_tasklet_kthread", &irq);
    if (ret) {
        pr_err("Failed to request IRQ\n");
        kthread_stop(worker_thread);
        return ret;
    }

    pr_info("Module Inserted: IRQ → Tasklet → Kthread Flow ready\n");
    return 0;
}

static void __exit irq_tasklet_kthread_exit(void)
{
    struct pkt_data *pkt, *tmp;

    free_irq(irq, &irq);
    tasklet_kill(&packet_tasklet);
    kthread_stop(worker_thread);

    spin_lock(&q_lock);
    list_for_each_entry_safe(pkt, tmp, &pkt_queue, list) {
        list_del(&pkt->list);
        kfree(pkt);
    }
    spin_unlock(&q_lock);

    pr_info("Module Removed: Clean exit\n");
}

module_init(irq_tasklet_kthread_init);
module_exit(irq_tasklet_kthread_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Bhargavi");
MODULE_DESCRIPTION("True IRQ → Tasklet → Kthread Packet Processing");

