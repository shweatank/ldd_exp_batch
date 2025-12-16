/*
 * bcm2711_uart_ioremap.c - Simple PL011 UART driver using pure ioremap
 *                           RX via interrupt, TX via polling
 *
 * Target: Raspberry Pi 4B (BCM2711) - UART0 / PL011
 *
 * This creates /dev/bcm2711_uart0
 *
 * How it works:
 *  - Uses hard-coded physical base address (ioremap)
 *  - Uses hard-coded IRQ (module param override recommended)
 *  - Sets up PL011 for 115200 8N1
 *  - RX interrupt fills a kernel ring buffer
 *  - read() blocks until data is available (interrupt-driven)
 *  - write() polls TX FIFO (FR_TXFF) and pushes bytes
 *
 * NOTE:
 *  - You MUST disable the firmware/console use of this UART.
 *  - Adjust base/IRQ for your setup; values here are typical for RPi 4.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/io.h>
#include <linux/interrupt.h>
#include <linux/wait.h>
#include <linux/spinlock.h>
#include <linux/slab.h>

#define DRIVER_NAME     "bcm2711_uart_ioremap"
#define DEVICE_NAME     "bcm2711_uart0"

/* ---------- BCM2711 / PL011 UART physical base ---------- */
/*
 * On Raspberry Pi 4B, PL011 UART0 is typically at 0xFE201000.
 * Verify with:  cat /proc/iomem  | grep UART
 */
#define UART_PHYS_BASE_DEFAULT  0xFE201000ULL
#define UART_REG_SIZE           0x1000

/* ---------- IRQ number ---------- */
/*
 * The GIC interrupt number for PL011-UART0 on BCM2711 is platform-specific.
 * On many RPi4 kernels it's mapped to a Linux IRQ in the 100+ range.
 *
 * Find it by:
 *   cat /proc/interrupts | grep -i uart
 *
 * Then pass that as module param "uart_irq".
 */
#define UART_IRQ_DEFAULT  153   /* Example, likely needs adjustment */

/* Allow overriding base and IRQ via module parameters */
static unsigned long uart_phys_base = UART_PHYS_BASE_DEFAULT;
module_param(uart_phys_base, ulong, 0444);
MODULE_PARM_DESC(uart_phys_base, "Physical base address of PL011 UART");

static int uart_irq = UART_IRQ_DEFAULT;
module_param(uart_irq, int, 0444);
MODULE_PARM_DESC(uart_irq, "IRQ number for PL011 UART");

/* ---------- PL011 Register Offsets ---------- */
#define UART_DR     0x00  /* Data Register */
#define UART_FR     0x18  /* Flag Register */
#define UART_IBRD   0x24  /* Integer Baud Rate Divisor */
#define UART_FBRD   0x28  /* Fractional Baud Rate Divisor */
#define UART_LCRH   0x2C  /* Line Control Register */
#define UART_CR     0x30  /* Control Register */
#define UART_IMSC   0x38  /* Interrupt Mask Set/Clear */
#define UART_RIS    0x3C  /* Raw Interrupt Status */
#define UART_MIS    0x40  /* Masked Interrupt Status */
#define UART_ICR    0x44  /* Interrupt Clear Register */

/* ---------- PL011 Bit Definitions ---------- */
#define UART_FR_TXFF    (1 << 5)    /* Transmit FIFO full */
#define UART_FR_RXFE    (1 << 4)    /* Receive FIFO empty */

#define UART_CR_UARTEN  (1 << 0)
#define UART_CR_TXE     (1 << 8)
#define UART_CR_RXE     (1 << 9)

#define UART_LCRH_FEN   (1 << 4)    /* Enable FIFOs */
#define UART_LCRH_WLEN_8 (3 << 5)   /* 8-bit word length */

#define UART_IMSC_RXIM  (1 << 4)    /* Receive interrupt mask */
#define UART_IMSC_RTIM  (1 << 6)    /* Receive timeout mask */

#define UART_ICR_RXIC   (1 << 4)
#define UART_ICR_RTIC   (1 << 6)

/* ---------- Simple RX Ring Buffer ---------- */
#define RX_BUF_SIZE 4096

struct uart_dev {
    void __iomem   *base;
    dev_t           devt;
    struct cdev     cdev;
    struct class   *class;

    /* RX ring buffer */
    unsigned char   rx_buf[RX_BUF_SIZE];
    size_t          rx_head;
    size_t          rx_tail;
    spinlock_t      rx_lock;
    wait_queue_head_t rx_wq;
};

static struct uart_dev *g_uart;

/* ---------- Register Access Helpers ---------- */

static inline u32 uart_read_inline(struct uart_dev *ud, u32 off)
{
    return readl(ud->base + off);
}

static inline void uart_write_inline(struct uart_dev *ud, u32 off, u32 val)
{
    writel(val, ud->base + off);
}

/* ---------- RX Ring Buffer Helpers ---------- */

static bool rx_buf_is_empty(struct uart_dev *ud)
{
    return ud->rx_head == ud->rx_tail;
}

static bool rx_buf_is_full(struct uart_dev *ud)
{
    return ((ud->rx_head + 1) % RX_BUF_SIZE) == ud->rx_tail;
}

static void rx_buf_push(struct uart_dev *ud, unsigned char ch)
{
    if (!rx_buf_is_full(ud)) {
        ud->rx_buf[ud->rx_head] = ch;
        ud->rx_head = (ud->rx_head + 1) % RX_BUF_SIZE;
    }
    /* If full, we silently drop bytes; could add stats/logging here */
}

static int rx_buf_pop(struct uart_dev *ud)
{
    int ret = -1;

    if (!rx_buf_is_empty(ud)) {
        ret = ud->rx_buf[ud->rx_tail];
        ud->rx_tail = (ud->rx_tail + 1) % RX_BUF_SIZE;
    }
    return ret;
}

/* ---------- IRQ Handler (RX interrupt) ---------- */

static irqreturn_t uart_irq_handler(int irq, void *dev_id)
{
    struct uart_dev *ud = dev_id;
    unsigned long flags;
    u32 mis;
    int woke = 0;

    /* Read masked interrupt status */
    mis = uart_read_inline(ud, UART_MIS);

    if (!(mis & (UART_IMSC_RXIM | UART_IMSC_RTIM)))
        return IRQ_NONE;

    /* Drain RX FIFO into ring buffer */
    spin_lock_irqsave(&ud->rx_lock, flags);
    while (!(uart_read_inline(ud, UART_FR) & UART_FR_RXFE)) {
        u32 dr = uart_read_inline(ud, UART_DR);
        rx_buf_push(ud, (unsigned char)(dr & 0xFF));
        woke = 1;
    }
    spin_unlock_irqrestore(&ud->rx_lock, flags);

    /* Clear interrupt flags */
    uart_write_inline(ud, UART_ICR, UART_ICR_RXIC | UART_ICR_RTIC);

    if (woke)
        wake_up_interruptible(&ud->rx_wq);

    return IRQ_HANDLED;
}

/* ---------- UART Init / Deinit ---------- */

static void uart_hw_init(struct uart_dev *ud)
{
    u32 val;

    /* Disable UART before configuration */
    uart_write_inline(ud, UART_CR, 0);

    /*
     * Set baud rate to ~115200 assuming UARTCLK ~ 48 MHz
     * BRD = UARTCLK / (16 * baud)
     * For 48 MHz and 115200:
     * BRD ≈ 26.0417 => IBRD = 26, FBRD ≈ 3
     * These may need tuning depending on your clock.
     */
    uart_write_inline(ud, UART_IBRD, 26);
    uart_write_inline(ud, UART_FBRD, 3);

    /* 8N1, FIFOs enabled */
    uart_write_inline(ud, UART_LCRH, UART_LCRH_WLEN_8 | UART_LCRH_FEN);

    /* Enable RX + TX + UART */
    uart_write_inline(ud, UART_CR, UART_CR_UARTEN | UART_CR_TXE | UART_CR_RXE);

    /* Enable RX + RX timeout interrupts */
    val = UART_IMSC_RXIM | UART_IMSC_RTIM;
    uart_write_inline(ud, UART_IMSC, val);
}

static void uart_hw_deinit(struct uart_dev *ud)
{
    /* Disable interrupts and UART */
    uart_write_inline(ud, UART_IMSC, 0);
    uart_write_inline(ud, UART_CR, 0);
}

/* ---------- File Operations ---------- */

static int uart_open(struct inode *inode, struct file *file)
{
    file->private_data = g_uart;
    return 0;
}

static int uart_release(struct inode *inode, struct file *file)
{
    return 0;
}

static ssize_t uart_read(struct file *file, char __user *buf,
                         size_t count, loff_t *ppos)
{
    struct uart_dev *ud = file->private_data;
    unsigned long flags;
    ssize_t copied = 0;
    int ch;

    if (!ud)
        return -ENODEV;

    /* Block until at least one byte is available */
    if (wait_event_interruptible(ud->rx_wq, !rx_buf_is_empty(ud)))
        return -ERESTARTSYS;

    while (copied < count) {
        spin_lock_irqsave(&ud->rx_lock, flags);
        ch = rx_buf_pop(ud);
        spin_unlock_irqrestore(&ud->rx_lock, flags);

        if (ch < 0)
            break; /* buffer empty */

        if (put_user((unsigned char)ch, buf + copied))
            return -EFAULT;

        copied++;
    }

    return copied;
}

static ssize_t uart_write(struct file *file, const char __user *buf,
                          size_t count, loff_t *ppos)
{
    struct uart_dev *ud = file->private_data;
    size_t i;
    unsigned char ch;

    if (!ud)
        return -ENODEV;

    for (i = 0; i < count; i++) {
        if (get_user(ch, buf + i))
            return -EFAULT;

        /* Wait until TX FIFO is not full */
        while (uart_read_inline(ud, UART_FR) & UART_FR_TXFF)
            cpu_relax();

        uart_write_inline(ud, UART_DR, ch);
    }

    return count;
}

static const struct file_operations uart_fops = {
    .owner          = THIS_MODULE,
    .open           = uart_open,
    .release        = uart_release,
    .read           = uart_read,
    .write          = uart_write,
};

/* ---------- Module Init / Exit ---------- */

static int __init uart_mod_init(void)
{
    int ret;
    struct device *dev;

    pr_info(DRIVER_NAME ": init, phys_base=0x%lx, irq=%d\n",
            uart_phys_base, uart_irq);

    g_uart = kzalloc(sizeof(*g_uart), GFP_KERNEL);
    if (!g_uart)
        return -ENOMEM;

    spin_lock_init(&g_uart->rx_lock);
    init_waitqueue_head(&g_uart->rx_wq);

    /* ioremap the UART registers */
    g_uart->base = ioremap(uart_phys_base, UART_REG_SIZE);
    if (!g_uart->base) {
        pr_err(DRIVER_NAME ": ioremap failed\n");
        ret = -ENOMEM;
        goto err_free;
    }

    /* Allocate char device number */
    ret = alloc_chrdev_region(&g_uart->devt, 0, 1, DRIVER_NAME);
    if (ret) {
        pr_err(DRIVER_NAME ": alloc_chrdev_region failed\n");
        goto err_unmap;
    }

    cdev_init(&g_uart->cdev, &uart_fops);
    g_uart->cdev.owner = THIS_MODULE;

    ret = cdev_add(&g_uart->cdev, g_uart->devt, 1);
    if (ret) {
        pr_err(DRIVER_NAME ": cdev_add failed\n");
        goto err_unregister;
    }

    g_uart->class = class_create(DRIVER_NAME);
    if (IS_ERR(g_uart->class)) {
        pr_err(DRIVER_NAME ": class_create failed\n");
        ret = PTR_ERR(g_uart->class);
        goto err_cdev;
    }

    dev = device_create(g_uart->class, NULL, g_uart->devt,
                        NULL, DEVICE_NAME);
    if (IS_ERR(dev)) {
        pr_err(DRIVER_NAME ": device_create failed\n");
        ret = PTR_ERR(dev);
        goto err_class;
    }

    /* Request IRQ */
    ret = request_irq(uart_irq, uart_irq_handler, 0,
                      DRIVER_NAME, g_uart);
    if (ret) {
        pr_err(DRIVER_NAME ": request_irq(%d) failed: %d\n",
               uart_irq, ret);
        goto err_device;
    }

    /* Init UART HW */
    uart_hw_init(g_uart);

    pr_info(DRIVER_NAME ": loaded, /dev/%s ready\n", DEVICE_NAME);
    return 0;

err_device:
    device_destroy(g_uart->class, g_uart->devt);
err_class:
    class_destroy(g_uart->class);
err_cdev:
    cdev_del(&g_uart->cdev);
err_unregister:
    unregister_chrdev_region(g_uart->devt, 1);
err_unmap:
    iounmap(g_uart->base);
err_free:
    kfree(g_uart);
    g_uart = NULL;
    return ret;
}

static void __exit uart_mod_exit(void)
{
    if (!g_uart)
        return;

    uart_hw_deinit(g_uart);
    free_irq(uart_irq, g_uart);

    device_destroy(g_uart->class, g_uart->devt);
    class_destroy(g_uart->class);
    cdev_del(&g_uart->cdev);
    unregister_chrdev_region(g_uart->devt, 1);

    iounmap(g_uart->base);
    kfree(g_uart);
    g_uart = NULL;

    pr_info(DRIVER_NAME ": unloaded\n");
}

module_init(uart_mod_init);
module_exit(uart_mod_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("techdhaba");
MODULE_DESCRIPTION("Pure ioremap-based BCM2711 PL011 UART driver (RX interrupt, TX polling)");
MODULE_VERSION("1.0");
