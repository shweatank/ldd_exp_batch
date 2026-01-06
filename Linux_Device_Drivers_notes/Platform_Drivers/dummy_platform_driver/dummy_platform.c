#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/of.h>

/* 1. The Probe Function: Runs when the DTS and Driver match */
static int magic_probe(struct platform_device *pdev) {
    pr_info("Probe called: Device Match Found!\n");
    return 0;
}

/* 2. The Remove Function: Runs when the device or driver is removed */
static void magic_remove(struct platform_device *pdev) {
    pr_info("Remove called: Device is gone. Goodbye!\n");
}

/* 3. The Match Table: This is the "Key" that matches the DTS "Lock" */
static const struct of_device_id magic_ids[] = {
    { .compatible = "rpi, dummy_dts", },
    { /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, magic_ids);

/* 4. The Platform Driver Structure */
static struct platform_driver magic_driver = {
    .probe = magic_probe,
    .remove = magic_remove,
    .driver = {
        .name = "dummy_dts",
        .of_match_table = magic_ids,
    },
};


static int __init my_init(void)
{
    return platform_driver_register(&magic_driver);
}

static void __exit my_exit(void)
{
    platform_driver_unregister(&magic_driver);
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A simple DT matching driver");

