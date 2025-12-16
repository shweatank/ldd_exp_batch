#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/blkdev.h>
#include <linux/blk-mq.h>
#include <linux/blkpg.h>
#include <linux/vmalloc.h>
#include <linux/hdreg.h>
#include <linux/version.h>

#define DRV_NAME        "td_ramblock"
#define DRV_DISK_NAME   "tdram0"
#define TD_SECTOR_SIZE  512

static int td_mb = 64;                 // default size
module_param(td_mb, int, 0444);        // parameter name = td_mb, type = int
MODULE_PARM_DESC(td_mb, "Size of RAM disk in MB");
struct td_ramdisk {
    sector_t capacity_sectors;
    u8 *data;

    struct blk_mq_tag_set tag_set;
    struct request_queue *queue;
    struct gendisk *gd;
};

static int td_major;
static struct td_ramdisk *td_dev;

/* ---------- block operations ---------- */

static int td_open(struct gendisk *disk, blk_mode_t mode)
{
    return 0;
}

static void td_release(struct gendisk *disk)
{
}

static int td_getgeo(struct block_device *bdev, struct hd_geometry *geo)
{
    struct td_ramdisk *dev = bdev->bd_disk->private_data;
    sector_t sectors = dev->capacity_sectors;

    geo->heads     = 4;
    geo->sectors   = 16;
    geo->cylinders = sectors / (4 * 16);
    geo->start     = 0;
    return 0;
}

static const struct block_device_operations td_bdops = {
    .owner   = THIS_MODULE,
    .open    = td_open,
    .release = td_release,
    .getgeo  = td_getgeo,
};

/* ---------- data transfer ---------- */

static int td_transfer(struct td_ramdisk *dev, struct request *rq)
{
    struct req_iterator iter;
    struct bio_vec bvec;
    loff_t pos = blk_rq_pos(rq) * TD_SECTOR_SIZE;
    loff_t max = dev->capacity_sectors * TD_SECTOR_SIZE;

    rq_for_each_segment(bvec, rq, iter) {
        void *kaddr = kmap_local_page(bvec.bv_page);
        void *buf  = kaddr + bvec.bv_offset;
        size_t len = bvec.bv_len;

        if (pos >= max) {
            kunmap_local(kaddr);
            return -EIO;
        }
        if (pos + len > max)
            len = max - pos;

        if (rq_data_dir(rq) == WRITE)
            memcpy(dev->data + pos, buf, len);
        else
            memcpy(buf, dev->data + pos, len);

        kunmap_local(kaddr);
        pos += len;
    }
    return 0;
}

static blk_status_t td_queue_rq(struct blk_mq_hw_ctx *hctx,
                                const struct blk_mq_queue_data *bd)
{
    struct request *rq = bd->rq;
    struct td_ramdisk *dev = rq->q->queuedata;

    blk_mq_start_request(rq);

    if (blk_rq_is_passthrough(rq)) {
        blk_mq_end_request(rq, BLK_STS_IOERR);
        return BLK_STS_OK;
    }

    if (td_transfer(dev, rq))
        blk_mq_end_request(rq, BLK_STS_IOERR);
    else
        blk_mq_end_request(rq, BLK_STS_OK);

    return BLK_STS_OK;
}

static const struct blk_mq_ops td_mq_ops = {
    .queue_rq = td_queue_rq,
};

/* ---------- initialization ---------- */

static int td_alloc_disk_and_queue(struct td_ramdisk *dev)
{
    int err;

    memset(&dev->tag_set, 0, sizeof(dev->tag_set));
    dev->tag_set.ops = &td_mq_ops;
    dev->tag_set.nr_hw_queues = 1;
    dev->tag_set.queue_depth = 128;
    dev->tag_set.numa_node = NUMA_NO_NODE;
    dev->tag_set.flags = BLK_MQ_F_SHOULD_MERGE;

    err = blk_mq_alloc_tag_set(&dev->tag_set);
    if (err)
        return err;

    dev->queue = blk_mq_init_queue(&dev->tag_set);
    if (IS_ERR(dev->queue)) {
        err = PTR_ERR(dev->queue);
        blk_mq_free_tag_set(&dev->tag_set);
        return err;
    }

    dev->queue->queuedata = dev;
    blk_queue_logical_block_size(dev->queue, TD_SECTOR_SIZE);

    dev->gd = blk_mq_alloc_disk(&dev->tag_set, dev);
    if (IS_ERR(dev->gd)) {
        err = PTR_ERR(dev->gd);
        blk_mq_destroy_queue(dev->queue);
        blk_mq_free_tag_set(&dev->tag_set);
        return err;
    }

    dev->gd->major = td_major;
    dev->gd->first_minor = 0;
    dev->gd->fops = &td_bdops;

    snprintf(dev->gd->disk_name, DISK_NAME_LEN, DRV_DISK_NAME);

    set_capacity(dev->gd, dev->capacity_sectors);

    err = add_disk(dev->gd);
    if (err) {
        put_disk(dev->gd);
        blk_mq_destroy_queue(dev->queue);
        blk_mq_free_tag_set(&dev->tag_set);
        return err;
    }

    return 0;
}

static void td_free_disk_and_queue(struct td_ramdisk *dev)
{
    if (!dev)
        return;

    if (dev->gd) {
        del_gendisk(dev->gd);
        put_disk(dev->gd);
    }

    if (dev->queue)
        blk_mq_destroy_queue(dev->queue);

    blk_mq_free_tag_set(&dev->tag_set);
}

/* ---------- module init/exit ---------- */

static int __init td_init(void)
{
    size_t bytes;
    int err;

    td_dev = kzalloc(sizeof(*td_dev), GFP_KERNEL);
    if (!td_dev)
        return -ENOMEM;

    bytes = td_mb * 1024ULL * 1024ULL;
    td_dev->capacity_sectors = bytes / TD_SECTOR_SIZE;

    td_dev->data = vmalloc(bytes);
    if (!td_dev->data) {
        kfree(td_dev);
        return -ENOMEM;
    }
    memset(td_dev->data, 0, bytes);

    td_major = register_blkdev(0, DRV_NAME);
    if (td_major < 0) {
        vfree(td_dev->data);
        kfree(td_dev);
        return td_major;
    }

    err = td_alloc_disk_and_queue(td_dev);
    if (err) {
        unregister_blkdev(td_major, DRV_NAME);
        vfree(td_dev->data);
        kfree(td_dev);
        return err;
    }

    pr_info(DRV_NAME ": loaded, size %d MB\n", td_mb);
    return 0;
}

static void __exit td_exit(void)
{
    td_free_disk_and_queue(td_dev);
    unregister_blkdev(td_major, DRV_NAME);
    vfree(td_dev->data);
    kfree(td_dev);
    pr_info(DRV_NAME ": unloaded\n");
}

module_init(td_init);
module_exit(td_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("TechDhaba");
MODULE_DESCRIPTION("RAM-backed block device using blk-mq");

