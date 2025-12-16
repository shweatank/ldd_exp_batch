#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/io.h>
#include <linux/slab.h>
#include <linux/mutex.h>
#include <linux/ioctl.h>

#define DRV_NAME        "td_spi0_ioremap"

/*
 * SPI0 base:
 *   Legacy: 0x7e204000
 *   ARM low-peripheral: 0xfe204000
 */
#define SPI0_BASE_PHYS  0xFE204000
#define SPI0_REG_SIZE   0x100

/* SPI0 Register offsets (BCM2835/BCM2711) */
#define SPI_CS          0x00
#define SPI_FIFO        0x04
#define SPI_CLK         0x08
#define SPI_DLEN        0x0c
#define SPI_LTOH        0x10
#define SPI_DC          0x14

/* SPI_CS bits */
#define CS_LEN_LONG     (1 << 25)
#define CS_DMA_LEN      (1 << 24)
#define CS_CSPOL2       (1 << 23)
#define CS_CSPOL1       (1 << 22)
#define CS_CSPOL0       (1 << 21)
#define CS_RXF          (1 << 20)
#define CS_RXR          (1 << 19)
#define CS_TXD          (1 << 18)
#define CS_RXD          (1 << 17)
#define CS_DONE         (1 << 16)
#define CS_TE_EN        (1 << 15)
#define CS_LMONO        (1 << 14)
#define CS_LEN          (1 << 13)
#define CS_REN          (1 << 12)
#define CS_ADCS         (1 << 11)
#define CS_INTR         (1 << 10)
#define CS_INTD         (1 << 9)
#define CS_DMAEN        (1 << 8)
#define CS_TA           (1 << 7)
#define CS_CSPOL        (1 << 6)
#define CS_CLEAR_RX     (1 << 5)
#define CS_CLEAR_TX     (1 << 4)
#define CS_CPOL         (1 << 3)
#define CS_CPHA         (1 << 2)
#define CS_CS_MASK      0x3    /* bits 1:0 */

/* Clear both FIFOs helper */
#define CS_CLEAR_BOTH   (CS_CLEAR_RX | CS_CLEAR_TX)

/* Simple safe clock: core_clk / 250 => ~1 MHz for 250 MHz core */
#define SPI_CLK_DIV_1MHZ  250

#define TD_SPI_MAX_XFER  256

struct td_spi_xfer {
    __u32 len;
    __u8  tx[TD_SPI_MAX_XFER];
    __u8  rx[TD_SPI_MAX_XFER];
};

#define TD_SPI_IOC_MAGIC 's'
#define TD_SPI_IOC_XFER  _IOWR(TD_SPI_IOC_MAGIC, 1, struct td_spi_xfer)

struct td_spi0 {
    void __iomem *base;
    dev_t         devt;
    struct cdev   cdev;
    struct class *class;
    struct device *dev;
    struct mutex  lock;
};

static struct td_spi0 *td_spi;

/* Helpers */
static inline u32 spi_read(struct td_spi0 *spi, u32 reg)
{
    return readl(spi->base + reg);
}

static inline void spi_write(struct td_spi0 *spi, u32 reg, u32 val)
{
    writel(val, spi->base + reg);
}

/* Configure SPI0 for mode 0, CS0, ~1 MHz */
static void td_spi0_hw_init(struct td_spi0 *spi)
{
    u32 cs;

    /* Disable transfer, clear FIFOs */
    cs = spi_read(spi, SPI_CS);
    cs &= ~(CS_TA);
    cs |= CS_CLEAR_BOTH;
    spi_write(spi, SPI_CS, cs);

    /* Clock divider */
    spi_write(spi, SPI_CLK, SPI_CLK_DIV_1MHZ);

    /* Mode 0: CPOL=0, CPHA=0, CS=0 */
    cs = 0;
    cs |= 0;              /* CPOL=0, CPHA=0 */
    cs |= 0;              /* CS=0 */
    cs |= CS_CLEAR_BOTH;  /* clear FIFOs */
    spi_write(spi, SPI_CS, cs);
}

/* Full-duplex transfer: len bytes from tx -> MOSI, rx <- MISO */
static int td_spi0_xfer(struct td_spi0 *spi,
                        const u8 *tx, u8 *rx, u32 len)
{
    u32 cs;
    u32 remaining = len;
    u32 rx_remaining = len;
    unsigned int timeout;

    if (!len || len > TD_SPI_MAX_XFER)
        return -EINVAL;

    /* Clear FIFOs, DONE, then start transfer */
    cs = spi_read(spi, SPI_CS);
    cs |= CS_CLEAR_BOTH;
    cs |= CS_DONE; /* writing 1 clears DONE */
    spi_write(spi, SPI_CS, cs);

    /* Set DLEN if you want to use it (not strictly required in polled mode) */
    spi_write(spi, SPI_DLEN, len);

    /* Enable transfer */
    cs &= ~(CS_CS_MASK);
    cs |= 0;          /* CS0 */
    cs |= CS_TA;      /* Transfer active */
    spi_write(spi, SPI_CS, cs);

    timeout = 1000000;

    while ((remaining || rx_remaining) && timeout--) {
        u32 status = spi_read(spi, SPI_CS);

        /* TX side: feed data while TXD is set */
        while (remaining && (status & CS_TXD)) {
            writel(*tx++, spi->base + SPI_FIFO);
            remaining--;
            status = spi_read(spi, SPI_CS);
        }

        /* RX side: drain data while RXD is set */
        while (rx_remaining && (status & CS_RXD)) {
            u32 v = readl(spi->base + SPI_FIFO);
            if (rx)
                *rx++ = (u8)v;
            rx_remaining--;
            status = spi_read(spi, SPI_CS);
        }

        /* Break if DONE and nothing left */
        if (!(remaining || rx_remaining) || (status & CS_DONE))
            break;

        cpu_relax();
    }

    /* Clear TA, DONE */
    cs = spi_read(spi, SPI_CS);
    cs &= ~CS_TA;
    cs |= CS_DONE;
    spi_write(spi, SPI_CS, cs);

    if (!timeout)
        return -ETIMEDOUT;

    return len;
}

/* ----- Character device ops ----- */

static int td_spi0_open(struct inode *inode, struct file *filp)
{
    filp->private_data = td_spi;
    return 0;
}

/* Simple TX-only transfer via write() (MISO ignored) */
static ssize_t td_spi0_write(struct file *filp, const char __user *ubuf,
                             size_t len, loff_t *ppos)
{
    struct td_spi0 *spi = filp->private_data;
    u8 *kbuf;
    int ret;

    if (!len || len > TD_SPI_MAX_XFER)
        return -EINVAL;

    kbuf = kmalloc(len, GFP_KERNEL);
    if (!kbuf)
        return -ENOMEM;

    if (copy_from_user(kbuf, ubuf, len)) {
        kfree(kbuf);
        return -EFAULT;
    }

    mutex_lock(&spi->lock);
    ret = td_spi0_xfer(spi, kbuf, NULL, len);
    mutex_unlock(&spi->lock);

    kfree(kbuf);

    if (ret < 0)
        return ret;

    return len;
}

static long td_spi0_ioctl(struct file *filp,
                          unsigned int cmd, unsigned long arg)
{
    struct td_spi0 *spi = filp->private_data;
    struct td_spi_xfer xfer;
    int ret;

    if (_IOC_TYPE(cmd) != TD_SPI_IOC_MAGIC)
        return -ENOTTY;

    switch (cmd) {
    case TD_SPI_IOC_XFER:
        if (copy_from_user(&xfer, (void __user *)arg, sizeof(xfer)))
            return -EFAULT;

        if (!xfer.len || xfer.len > TD_SPI_MAX_XFER)
            return -EINVAL;

        mutex_lock(&spi->lock);
        ret = td_spi0_xfer(spi, xfer.tx, xfer.rx, xfer.len);
        mutex_unlock(&spi->lock);

        if (ret < 0)
            return ret;

        if (copy_to_user((void __user *)arg, &xfer, sizeof(xfer)))
            return -EFAULT;

        return 0;

    default:
        return -ENOTTY;
    }
}

static const struct file_operations td_spi0_fops = {
    .owner          = THIS_MODULE,
    .open           = td_spi0_open,
    .write          = td_spi0_write,
    .unlocked_ioctl = td_spi0_ioctl,
    .llseek         = no_llseek,
};

/* ----- Module init/exit ----- */

static int __init td_spi0_init(void)
{
    int ret;
    struct device *dev;

    td_spi = kzalloc(sizeof(*td_spi), GFP_KERNEL);
    if (!td_spi)
        return -ENOMEM;

    mutex_init(&td_spi->lock);

    td_spi->base = ioremap(SPI0_BASE_PHYS, SPI0_REG_SIZE);
    if (!td_spi->base) {
        pr_err(DRV_NAME ": ioremap failed\n");
        ret = -ENOMEM;
        goto err_free;
    }

    td_spi0_hw_init(td_spi);

    ret = alloc_chrdev_region(&td_spi->devt, 0, 1, DRV_NAME);
    if (ret)
        goto err_unmap;

    cdev_init(&td_spi->cdev, &td_spi0_fops);
    td_spi->cdev.owner = THIS_MODULE;

    ret = cdev_add(&td_spi->cdev, td_spi->devt, 1);
    if (ret)
        goto err_unregister;

    td_spi->class = class_create(THIS_MODULE, DRV_NAME);
    if (IS_ERR(td_spi->class)) {
        ret = PTR_ERR(td_spi->class);
        goto err_cdev;
    }

    dev = device_create(td_spi->class, NULL, td_spi->devt, NULL,
                        "td_spi0");
    if (IS_ERR(dev)) {
        ret = PTR_ERR(dev);
        goto err_class;
    }

    td_spi->dev = dev;

    pr_info(DRV_NAME ": loaded, /dev/td_spi0 mapped at 0x%pa\n",
            &((phys_addr_t)SPI0_BASE_PHYS));
    return 0;

err_class:
    class_destroy(td_spi->class);
err_cdev:
    cdev_del(&td_spi->cdev);
err_unregister:
    unregister_chrdev_region(td_spi->devt, 1);
err_unmap:
    iounmap(td_spi->base);
err_free:
    kfree(td_spi);
    return ret;
}

static void __exit td_spi0_exit(void)
{
    device_destroy(td_spi->class, td_spi->devt);
    class_destroy(td_spi->class);
    cdev_del(&td_spi->cdev);
    unregister_chrdev_region(td_spi->devt, 1);
    iounmap(td_spi->base);
    kfree(td_spi);
    pr_info(DRV_NAME ": unloaded\n");
}

module_init(td_spi0_init);
module_exit(td_spi0_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("techdhaba");
MODULE_DESCRIPTION("BCM2711 SPI0 simple ioremap-based SPI driver");
