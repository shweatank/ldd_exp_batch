// SPDX-License-Identifier: GPL-2.0
//
// Simple x86-friendly Linux power management demo driver
// Shows suspend/resume callbacks using dev_pm_ops
//
// Author: TechDhaba

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/pm.h>

#define DRV_NAME "pm_demo"

// ----------------- Power Management Callbacks -----------------

static int pm_demo_suspend(struct device *dev)
{
    pr_info("%s: suspend() called\n", DRV_NAME);

    /*
     * TODO: In a real driver you would:
     *  - Stop I/O
     *  - Save device registers
     *  - Put device into low power state
     */

    return 0;   // 0 = success, non-zero blocks suspend
}

static int pm_demo_resume(struct device *dev)
{
    pr_info("%s: resume() called\n", DRV_NAME);

    /*
     * TODO: In a real driver you would:
     *  - Restore registers
     *  - Re-enable interrupts
     *  - Restart I/O
     */

    return 0;
}

static int pm_demo_freeze(struct device *dev)
{
    pr_info("%s: freeze() (hibernation) called\n", DRV_NAME);
    return 0;
}

static int pm_demo_thaw(struct device *dev)
{
    pr_info("%s: thaw() called\n", DRV_NAME);
    return 0;
}

static int pm_demo_poweroff(struct device *dev)
{
    pr_info("%s: poweroff() called\n", DRV_NAME);
    return 0;
}

static int pm_demo_restore(struct device *dev)
{
    pr_info("%s: restore() (from hibernation) called\n", DRV_NAME);
    return 0;
}

// Link callbacks in dev_pm_ops
static const struct dev_pm_ops pm_demo_pm_ops = {
    .suspend  = pm_demo_suspend,
    .resume   = pm_demo_resume,
    .freeze   = pm_demo_freeze,
    .thaw     = pm_demo_thaw,
    .poweroff = pm_demo_poweroff,
    .restore  = pm_demo_restore,
};

// ----------------- Platform Driver Skeleton -----------------

static int pm_demo_probe(struct platform_device *pdev)
{
    pr_info("%s: probe() called for device '%s'\n",
            DRV_NAME, dev_name(&pdev->dev));
    /*
     * In real HW driver: ioremap, request_irq, allocate data, etc.
     */
    return 0;
}

static int pm_demo_remove(struct platform_device *pdev)
{
    pr_info("%s: remove() called for device '%s'\n",
            DRV_NAME, dev_name(&pdev->dev));
    /*
     * In real HW driver: free IRQ, iounmap, kfree, etc.
     */
    return 0;
}

static struct platform_driver pm_demo_driver = {
    .probe  = pm_demo_probe,
    .remove = pm_demo_remove,
    .driver = {
        .name = DRV_NAME,
        .pm   = &pm_demo_pm_ops,   // attach PM ops here
    },
};

// ----------------- Optional: Dummy Platform Device -----------------
//
// On many x86 systems you won’t have a DT/platform device entry for this
// name, so for demo we create a software-only platform_device.
//
// NOTE: In real usage, device comes from ACPI/DT/board code.

static struct platform_device *pm_demo_pdev;

static int __init pm_demo_init(void)
{
    int ret;

    pr_info("%s: init\n", DRV_NAME);

    // 1) Register the driver
    ret = platform_driver_register(&pm_demo_driver);
    if (ret) {
        pr_err("%s: platform_driver_register failed: %d\n", DRV_NAME, ret);
        return ret;
    }

    // 2) Create a dummy device so probe() is called
    pm_demo_pdev = platform_device_register_simple(DRV_NAME, -1, NULL, 0);
    if (IS_ERR(pm_demo_pdev)) {
        pr_err("%s: platform_device_register_simple failed\n", DRV_NAME);
        ret = PTR_ERR(pm_demo_pdev);
        platform_driver_unregister(&pm_demo_driver);
        return ret;
    }

    pr_info("%s: driver and dummy device registered\n", DRV_NAME);
    return 0;
}

static void __exit pm_demo_exit(void)
{
    pr_info("%s: exit\n", DRV_NAME);

    // Remove dummy device first
    if (pm_demo_pdev && !IS_ERR(pm_demo_pdev))
        platform_device_unregister(pm_demo_pdev);

    // Unregister driver
    platform_driver_unregister(&pm_demo_driver);
}

module_init(pm_demo_init);
module_exit(pm_demo_exit);

MODULE_AUTHOR("TechDhaba");
MODULE_DESCRIPTION("Basic x86-friendly Power Management Demo Driver");
MODULE_LICENSE("GPL");
