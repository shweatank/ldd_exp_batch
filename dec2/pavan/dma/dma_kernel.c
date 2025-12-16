#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/io.h>
#include <linux/slab.h>
#include <linux/delay.h>

#define DRV_NAME        "bcm2711_dma_mem_ioremap"
#define DEV_NAME        "td_dma_mem"

#define DMA_BASE_PHYS   0xFE007000
#define DMA_REG_SIZE    0x1000
#define DMA_CHAN        5

#define DMA_CH_BASE     (DMA_CHAN * 0x100)

#define DMA_CS          0x00
#define DMA_CONBLK_AD   0x04
#define DMA_DEBUG       0x20

#define DMA_CS_RESET    (1 << 31)
#define DMA_CS_ABORT    (1 << 30)
#define DMA_CS_ACTIVE   (1 << 0)
#define DMA_CS_END      (1 << 1)
#define DMA_CS_INT      (1 << 2)
#define DMA_CS_ERR      (1 << 8)

#define DMA_LEN         4096

#define BUS_ADDR(x)     (x)

struct bcm2711_dma_cb {
    u32 ti;
    u32 source_ad;
    u32 dest_ad;
    u32 txfr_len;
    u32 stride;
    u32 nextconbk;
    u32 reserved1;
    u32 reserved2;
};

struct td_dma {
    void __iomem          *base;
    void __iomem          *chan_base;
    dev_t                  devt;
    struct cdev            cdev;
    struct class          *class;
    struct device         *dev;
    struct bcm2711_dma_cb *cb;
    dma_addr_t             cb_phys;
    u8                    *src;
    u8                    *dst;
    dma_addr_t             src_phys;
    dma_addr_t             dst_phys;
};

static struct td_dma *td_dma_dev;

static inline u32 dma_read(struct td_dma *d, u32 off)
{
    return readl(d->chan_base + off);
}

static inline void dma_write(struct td_dma *d, u32 off, u32 val)
{
    writel(val, d->chan_base + off);
}

static int td_dma_mem_run(struct td_dma *d)
{
    u32 cs;
    unsigned int i;
    unsigned int timeout = 1000000;

    for (i = 0; i < DMA_LEN; i++)
        d->src[i] = (u8)(i & 0xFF);

    for (i = 0; i < DMA_LEN; i++)
        d->dst[i] = 0;

    d->cb->ti = (1 << 8) | (1 << 4) | (1 << 3);
    d->cb->source_ad = BUS_ADDR(d->src_phys);
    d->cb->dest_ad = BUS_ADDR(d->dst_phys);
    d->cb->txfr_len = DMA_LEN;
    d->cb->stride = 0;
    d->cb->nextconbk = 0;
    d->cb->reserved1 = 0;
    d->cb->reserved2 = 0;

    dma_write(d, DMA_CS, DMA_CS_ABORT);
    udelay(10);
    dma_write(d, DMA_CS, DMA_CS_RESET);
    udelay(10);
    dma_write(d, DMA_CS, DMA_CS_INT | DMA_CS_END | DMA_CS_ERR);

    dma_write(d, DMA_CONBLK_AD, BUS_ADDR(d->cb_phys));
    dma_write(d, DMA_CS, DMA_CS_ACTIVE);

    while (timeout--) {
        cs = dma_read(d, DMA_CS);
        if (cs & DMA_CS_ERR)
            return -EIO;
        if (cs & DMA_CS_END)
            break;
        cpu_relax();
    }

    if (!timeout)
        return -ETIMEDOUT;

    for (i = 0; i < DMA_LEN; i++) {
        if (d->dst[i] != (u8)(i & 0xFF))
            return -EFAULT;
    }

    return 0;
}

static int td_dma_open(struct inode *inode, struct file *filp)
{
    filp->private_data = td_dma_dev;
    return 0;
}

static ssize_t td_dma_write(struct file *filp, const char __user *buf,
                            size_t len, loff_t *ppos)
{
    struct td_dma *d = filp->private_data;
    int ret;

    if (!d)
        return -ENODEV;

    ret = td_dma_mem_run(d);
    if (ret < 0)
        return ret;

    return len;
}

static const struct file_operations td_dma_fops = {
    .owner  = THIS_MODULE,
    .open   = td_dma_open,
    .write  = td_dma_write,
    .llseek = no_llseek,
};

static int __init td_dma_init(void)
{
    int ret;
    struct device *dev;
    phys_addr_t base = DMA_BASE_PHYS;

    td_dma_dev = kzalloc(sizeof(*td_dma_dev), GFP_KERNEL);
    if (!td_dma_dev)
        return -ENOMEM;

    td_dma_dev->base = ioremap(base, DMA_REG_SIZE);
    if (!td_dma_dev->base) {
        ret = -ENOMEM;
        goto err_free;
    }

    td_dma_dev->chan_base = td_dma_dev->base + DMA_CH_BASE;

    td_dma_dev->cb = (struct bcm2711_dma_cb *)kmalloc(sizeof(*td_dma_dev->cb), GFP_KERNEL | GFP_DMA);
    if (!td_dma_dev->cb) {
        ret = -ENOMEM;
        goto err_unmap;
    }

    td_dma_dev->src = (u8 *)kmalloc(DMA_LEN, GFP_KERNEL | GFP_DMA);
    if (!td_dma_dev->src) {
        ret = -ENOMEM;
        goto err_cb;
    }

    td_dma_dev->dst = (u8 *)kmalloc(DMA_LEN, GFP_KERNEL | GFP_DMA);
    if (!td_dma_dev->dst) {
        ret = -ENOMEM;
        goto err_src;
    }

    td_dma_dev->cb_phys = virt_to_phys(td_dma_dev->cb);
    td_dma_dev->src_phys = virt_to_phys(td_dma_dev->src);
    td_dma_dev->dst_phys = virt_to_phys(td_dma_dev->dst);

    ret = alloc_chrdev_region(&td_dma_dev->devt, 0, 1, DRV_NAME);
    if (ret)
        goto err_dst;

    cdev_init(&td_dma_dev->cdev, &td_dma_fops);
    td_dma_dev->cdev.owner = THIS_MODULE;

    ret = cdev_add(&td_dma_dev->cdev, td_dma_dev->devt, 1);
    if (ret)
        goto err_unregister;

    td_dma_dev->class = class_create(THIS_MODULE, DRV_NAME);
    if (IS_ERR(td_dma_dev->class)) {
        ret = PTR_ERR(td_dma_dev->class);
        goto err_cdev;
    }

    dev = device_create(td_dma_dev->class, NULL, td_dma_dev->devt, NULL, DEV_NAME);
    if (IS_ERR(dev)) {
        ret = PTR_ERR(dev);
        goto err_class;
    }

    td_dma_dev->dev = dev;

    pr_info(DRV_NAME ": loaded, /dev/%s, DMA base 0x%pa channel %d\n",
            DEV_NAME, &base, DMA_CHAN);
    return 0;

err_class:
    class_destroy(td_dma_dev->class);
err_cdev:
    cdev_del(&td_dma_dev->cdev);
err_unregister:
    unregister_chrdev_region(td_dma_dev->devt, 1);
err_dst:
    kfree(td_dma_dev->dst);
err_src:
    kfree(td_dma_dev->src);
err_cb:
    kfree(td_dma_dev->cb);
err_unmap:
    iounmap(td_dma_dev->base);
err_free:
    kfree(td_dma_dev);
    td_dma_dev = NULL;
    return ret;
}

static void __exit td_dma_exit(void)
{
    phys_addr_t base = DMA_BASE_PHYS;

    if (!td_dma_dev)
        return;

    device_destroy(td_dma_dev->class, td_dma_dev->devt);
    class_destroy(td_dma_dev->class);
    cdev_del(&td_dma_dev->cdev);
    unregister_chrdev_region(td_dma_dev->devt, 1);
    kfree(td_dma_dev->dst);
    kfree(td_dma_dev->src);
    kfree(td_dma_dev->cb);
    iounmap(td_dma_dev->base);
    kfree(td_dma_dev);
    td_dma_dev = NULL;

    pr_info(DRV_NAME ": unloaded, DMA base 0x%pa channel %d\n",
            &base, DMA_CHAN);
}

module_init(td_dma_init);
module_exit(td_dma_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("techdhaba");
MODULE_DESCRIPTION("BCM2711 simple memory-to-memory DMA driver using ioremap");
MODULE_VERSION("1.0");
