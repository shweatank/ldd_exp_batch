// td_ramblock.c - Simple RAM-backed block device for modern kernels
//
// Tested pattern against upstream docs and brd/null_blk style code.
// Uses blk-mq, blk_alloc_disk (NOT alloc_disk/blk_fetch_request).
//
// Build as an out-of-tree module and insmod on Raspberry Pi.
//
// Author: TechDhaba

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/blkdev.h>
#include <linux/blk-mq.h>
#include <linux/vmalloc.h>
#include <linux/hdreg.h>
#include <linux/version.h>

#define DRV_NAME        "td_ramblock"
#define DRV_DISK_NAME   "tdram0"

#define TD_SECTOR_SIZE  512    /* logical sector size */

static int td_mb = 16;         /* default size = 16 MB */
module_param(td_mb, int, 0444);
MODULE_PARM_DESC(td_mb, "Size of RAM disk in MB");

struct td_ramdisk {
    sector_t capacity_sectors;     /* total capacity in 512B sectors */
    u8       *data;                /* vmalloc'ed backing store */

    struct blk_mq_tag_set tag_set;
    struct gendisk       *gd;
};

static int td_major;
static struct td_ramdisk *td_dev;

/* ---------- Block device operations ---------- */

static int td_open(struct gendisk *gd, blk_mode_t mode)
{
    pr_info(DRV_NAME ": open\n");
    return 0;
}

static void td_release(struct gendisk *gd)
{
    pr_info(DRV_NAME ": release\n");
}

static int td_getgeo(struct block_device *bdev, struct hd_geometry *geo)
{
    struct td_ramdisk *dev = bdev->bd_disk->private_data;
    sector_t sectors = dev->capacity_sectors;
    sector_t cylinders = sectors / (4 * 16);

    geo->heads     = 4;
    geo->sectors   = 16;
    geo->cylinders = cylinders;
    geo->start     = 0;

    return 0;
}

static const struct block_device_operations td_bdops = {
    .owner   = THIS_MODULE,
    .open    = td_open,
    .release = td_release,
    .getgeo  = td_getgeo,
};

/* ---------- Request handling (blk-mq) ---------- */

static int td_transfer(struct td_ramdisk *dev, struct request *rq,
                       unsigned int *bytes_done)
{
    struct bio_vec bvec;
    struct req_iterator iter;
    loff_t pos;              /* byte offset into our backing store */
    loff_t disk_bytes;

    pos = (loff_t)blk_rq_pos(rq) * TD_SECTOR_SIZE;
    disk_bytes = (loff_t)dev->capacity_sectors * TD_SECTOR_SIZE;

    if (pos >= disk_bytes) {
        pr_err(DRV_NAME ": request beyond end of device (pos=%lld, size=%lld)\n",
               pos, (long long)blk_rq_bytes(rq));
        return -EIO;
    }

    rq_for_each_segment(bvec, rq, iter) {
        unsigned long len  = bvec.bv_len;
        void *iovec_mem = kmap_local_page(bvec.bv_page) + bvec.bv_offset;

        if (pos + len > disk_bytes)
            len = disk_bytes - pos;

        if (rq_data_dir(rq) == WRITE) {
            memcpy(dev->data + pos, iovec_mem, len);
        } else {
            memcpy(iovec_mem, dev->data + pos, len);
        }

        kunmap_local(iovec_mem);

        pos          += len;
        *bytes_done  += len;
    }

    return 0;
}

static blk_status_t td_queue_rq(struct blk_mq_hw_ctx *hctx,
                                const struct blk_mq_queue_data *bd)
{
    struct request *rq = bd->rq;
    struct td_ramdisk *dev = rq->q->queuedata;
    unsigned int bytes = 0;
    int ret;
    blk_status_t status = BLK_STS_OK;

    blk_mq_start_request(rq);

    if (blk_rq_is_passthrough(rq)) {
        pr_warn(DRV_NAME ": skipping non-fs request\n");
        status = BLK_STS_IOERR;
        goto out_end;
    }

    ret = td_transfer(dev, rq, &bytes);
    if (ret) {
        status = BLK_STS_IOERR;
        goto out_end;
    }

out_end:
    /*
     * blk_update_request() adjusts request's internal state and
     * returns non-zero if there is remaining data (should not happen
     * in our simple driver).
     */
    if (blk_update_request(rq, status, bytes))
        pr_err(DRV_NAME ": leftover data on request!\n");

    __blk_mq_end_request(rq, status);
    return BLK_STS_OK;
}

static const struct blk_mq_ops td_mq_ops = {
    .queue_rq = td_queue_rq,
};

/* ---------- Init / Exit helpers ---------- */

static int td_alloc_disk_and_queue(struct td_ramdisk *dev)
{
    int err;
    struct request_queue *q;
    struct gendisk *gd;

    /* 1) Setup tag_set for blk-mq */
    memset(&dev->tag_set, 0, sizeof(dev->tag_set));
    dev->tag_set.ops         = &td_mq_ops;
    dev->tag_set.nr_hw_queues = 1;
    dev->tag_set.queue_depth = 128;
    dev->tag_set.numa_node   = NUMA_NO_NODE;
    dev->tag_set.cmd_size    = 0;
    dev->tag_set.flags       = BLK_MQ_F_SHOULD_MERGE;

    err = blk_mq_alloc_tag_set(&dev->tag_set);
    if (err) {
        pr_err(DRV_NAME ": blk_mq_alloc_tag_set failed: %d\n", err);
        return err;
    }

    /* 2) Create request queue from tag_set */
    q = blk_mq_init_queue(&dev->tag_set);
    if (IS_ERR(q)) {
        err = PTR_ERR(q);
        pr_err(DRV_NAME ": blk_mq_init_queue failed: %d\n", err);
        goto out_free_tag_set;
    }

    q->queuedata = dev;
    blk_queue_logical_block_size(q, TD_SECTOR_SIZE);

    /* 3) Allocate gendisk (modern helper instead of alloc_disk) */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 10, 0)
    /*
     * Newer kernels have blk_alloc_disk(struct queue_limits *, int node)
     * and often drivers pass some queue_limits. For a simple RAM disk we
     * can use the queue's limits directly.
     */
    gd = blk_alloc_disk(&q->limits, NUMA_NO_NODE);
#else
    /*
     * Older 5.x / early 6.x: blk_alloc_disk(int node).
     * If your kernel complains about the prototype here, just switch to
     * the other branch accordingly.
     */
    gd = blk_alloc_disk(NUMA_NO_NODE);
#endif

    if (IS_ERR(gd)) {
        err = PTR_ERR(gd);
        pr_err(DRV_NAME ": blk_alloc_disk failed: %d\n", err);
        goto out_cleanup_queue;
    }

    dev->gd        = gd;
    dev->gd->major = td_major;
    dev->gd->first_minor = 0;
    dev->gd->minors = 1;
    dev->gd->fops  = &td_bdops;
    dev->gd->private_data = dev;
    dev->gd->queue = q;

    snprintf(dev->gd->disk_name, DISK_NAME_LEN, DRV_DISK_NAME);
    set_capacity(dev->gd, dev->capacity_sectors);
		
    int ret=add_disk(dev->gd);
    if(ret){
    	put_disk(dev->gd);
	return ret;
    }

    pr_info(DRV_NAME ": registered /dev/%s size=%llu sectors (%d MB)\n",
            dev->gd->disk_name,
            (unsigned long long)dev->capacity_sectors,
            td_mb);

    return 0;

out_cleanup_queue:
    //blk_cleanup_queue(q);
    blk_put_queue(q);
out_free_tag_set:
    blk_mq_free_tag_set(&dev->tag_set);
    return err;
}

static void td_free_disk_and_queue(struct td_ramdisk *dev)
{
    if (!dev)
        return;

    if (dev->gd) {
        del_gendisk(dev->gd);
        /* queue is dev->gd->queue */
        //blk_cleanup_queue(dev->gd->queue);
	blk_put_queue(dev->gd->queue);
        put_disk(dev->gd);
    }

    blk_mq_free_tag_set(&dev->tag_set);
}

/* ---------- Module init / exit ---------- */

static int __init td_init(void)
{
    int err;
    size_t bytes;

    if (td_mb <= 0)
        td_mb = 16;

    td_dev = kzalloc(sizeof(*td_dev), GFP_KERNEL);
    if (!td_dev)
        return -ENOMEM;

    bytes = (size_t)td_mb * 1024 * 1024;
    td_dev->capacity_sectors = bytes / TD_SECTOR_SIZE;

    td_dev->data = vmalloc(bytes);
    if (!td_dev->data) {
        pr_err(DRV_NAME ": vmalloc(%zu) failed\n", bytes);
        err = -ENOMEM;
        goto out_free_dev;
    }
    memset(td_dev->data, 0, bytes);

    td_major = register_blkdev(0, DRV_NAME);
    if (td_major < 0) {
        pr_err(DRV_NAME ": register_blkdev failed: %d\n", td_major);
        err = td_major;
        goto out_free_data;
    }

    err = td_alloc_disk_and_queue(td_dev);
    if (err)
        goto out_unreg_blkdev;

    pr_info(DRV_NAME ": module loaded, major=%d, size=%d MB\n",
            td_major, td_mb);
    return 0;

out_unreg_blkdev:
    unregister_blkdev(td_major, DRV_NAME);
out_free_data:
    vfree(td_dev->data);
out_free_dev:
    kfree(td_dev);
    return err;
}

static void __exit td_exit(void)
{
    size_t bytes;

    td_free_disk_and_queue(td_dev);

    unregister_blkdev(td_major, DRV_NAME);

    if (td_dev) {
        if (td_dev->data) {
            bytes = (size_t)td_mb * 1024 * 1024;
            vfree(td_dev->data);
        }
        kfree(td_dev);
    }

    pr_info(DRV_NAME ": module unloaded\n");
}

module_init(td_init);
module_exit(td_exit);

MODULE_AUTHOR("TechDhaba");
MODULE_DESCRIPTION("Simple RAM-backed block driver using blk-mq");
MODULE_LICENSE("GPL");
