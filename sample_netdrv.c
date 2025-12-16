#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/skbuff.h>

#define DRV_NAME "sample_netdrv"

/* -------------------------------------------------- */
/* TX path: Called when kernel sends a packet */
static netdev_tx_t sample_start_xmit(struct sk_buff *skb,
                                     struct net_device *dev)
{
    /* Update statistics */
    dev->stats.tx_packets++;
    dev->stats.tx_bytes += skb->len;

    pr_info("%s: TX packet len=%u\n", dev->name, skb->len);

    /* Drop packet (virtual NIC) */
    dev_kfree_skb(skb);

    return NETDEV_TX_OK;
}

/* -------------------------------------------------- */
/* Interface up */
static int sample_open(struct net_device *dev)
{
    pr_info("%s: interface opened\n", dev->name);
    netif_start_queue(dev);
    return 0;
}

/* -------------------------------------------------- */
/* Interface down */
static int sample_stop(struct net_device *dev)
{
    pr_info("%s: interface stopped\n", dev->name);
    netif_stop_queue(dev);
    return 0;
}

/* -------------------------------------------------- */
/* Netdevice operations */
static const struct net_device_ops sample_netdev_ops = {
    .ndo_open       = sample_open,
    .ndo_stop       = sample_stop,
    .ndo_start_xmit = sample_start_xmit,
};

/* -------------------------------------------------- */
/* Setup function */
static void sample_setup(struct net_device *dev)
{
    ether_setup(dev);

    dev->netdev_ops = &sample_netdev_ops;

    dev->flags |= IFF_NOARP;
    dev->features |= NETIF_F_HW_CSUM;

    eth_hw_addr_random(dev);
}

/* -------------------------------------------------- */
/* Global net_device pointer */
static struct net_device *sample_dev;

/* -------------------------------------------------- */
/* Module init */
static int __init sample_init(void)
{
    int ret;

    sample_dev = alloc_netdev(0, "sn%d", NET_NAME_UNKNOWN, sample_setup);
    if (!sample_dev)
        return -ENOMEM;

    ret = register_netdev(sample_dev);
    if (ret) {
        free_netdev(sample_dev);
        return ret;
    }

    pr_info(DRV_NAME ": loaded\n");
    return 0;
}

/* -------------------------------------------------- */
/* Module exit */
static void __exit sample_exit(void)
{
    unregister_netdev(sample_dev);
    free_netdev(sample_dev);
    pr_info(DRV_NAME ": unloaded\n");
}

module_init(sample_init);
module_exit(sample_exit);

MODULE_AUTHOR("Techdhaba");
MODULE_DESCRIPTION("Sample x86 Linux Network Driver");
MODULE_LICENSE("GPL");
