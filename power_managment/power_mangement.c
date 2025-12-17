//
// pm_ioremap.c - x86-friendly power management demo driver using ioremap
//
// This driver pretends there is a memory-mapped "power controller" block
// exposed by the chipset/SoC. It maps it with ioremap and integrates
// with suspend/resume (dev_pm_ops).
//
// DO NOT point this at real hardware addresses without vendor docs.
// Use this as a learning / lab example.
//
// Author: TechDhaba

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/io.h>
#include <linux/pm.h>
#include <linux/slab.h>
#include <linux/sysfs.h>

#define DRV_NAME "pm_ioremap_demo"

// ---------------------------------------------------------------------
// Fake power controller register layout (purely illustrative)
// ---------------------------------------------------------------------
//
// base + 0x00 : PM_CTRL     - control register
//     bit[0]   : DEEP_SLEEP_EN (1 = allow deeper low power state)
//     bit[1]   : WAKEUP_EN     (1 = device may wake system)
// base + 0x04 : PM_STATUS   - status register (not used much here)
//
// In real life, these would be defined by Intel/SoC vendor spec.
// ---------------------------------------------------------------------

#define PM_CTRL_OFFSET      0x00
#define PM_STATUS_OFFSET    0x04

#define PM_CTRL_DEEP_SLEEP_EN   BIT(0)
#define PM_CTRL_WAKEUP_EN       BIT(1)

// Per-device driver data
struct pm_ioremap_dev {
    void __iomem *base;
    u32           saved_ctrl;  // control register saved across suspend
};

static inline u32 pm_ioremap_readl(struct pm_ioremap_dev *pmdev, u32 off)
{
    return readl(pmdev->base + off);
}

static inline void pm_ioremap_writel(struct pm_ioremap_dev *pmdev, u32 off, u32 val)
{
    writel(val, pmdev->base + off);
}

// ---------------------------------------------------------------------
// Sysfs attribute: power_mode
//
//   cat power_mode
//   echo deep > power_mode
//   echo normal > power_mode
//
// This just toggles a bit in PM_CTRL for demo.
// ---------------------------------------------------------------------

static ssize_t power_mode_show(struct device *dev,
                               struct device_attribute *attr, char *buf)
{
    struct pm_ioremap_dev *pmdev = dev_get_drvdata(dev);
    u32 ctrl;

    if (!pmdev || !pmdev->base)
        return -ENODEV;

    ctrl = pm_ioremap_readl(pmdev, PM_CTRL_OFFSET);

    return sysfs_emit(buf, "PM_CTRL=0x%08x (DEEP_SLEEP_EN=%d, WAKEUP_EN=%d)\n",
                      ctrl,
                      !!(ctrl & PM_CTRL_DEEP_SLEEP_EN),
                      !!(ctrl & PM_CTRL_WAKEUP_EN));
}

static ssize_t power_mode_store(struct device *dev,
                                struct device_attribute *attr,
                                const char *buf, size_t count)
{
    struct pm_ioremap_dev *pmdev = dev_get_drvdata(dev);
    u32 ctrl;

    if (!pmdev || !pmdev->base)
        return -ENODEV;

    ctrl = pm_ioremap_readl(pmdev, PM_CTRL_OFFSET);

    if (sysfs_streq(buf, "deep")) {
        ctrl |= PM_CTRL_DEEP_SLEEP_EN;
        dev_info(dev, "%s: enabling deep sleep bit\n", DRV_NAME);
    } else if (sysfs_streq(buf, "normal")) {
        ctrl &= ~PM_CTRL_DEEP_SLEEP_EN;
        dev_info(dev, "%s: disabling deep sleep bit\n", DRV_NAME);
    } else {
        dev_warn(dev, "%s: unknown mode '%s' (use 'deep' or 'normal')\n",
                 DRV_NAME, buf);
        return -EINVAL;
    }

    pm_ioremap_writel(pmdev, PM_CTRL_OFFSET, ctrl);
    return count;
}

static DEVICE_ATTR_RW(power_mode);

static struct attribute *pm_ioremap_attrs[] = {
    &dev_attr_power_mode.attr,
    NULL,
};

static const struct attribute_group pm_ioremap_attr_group = {
    .attrs = pm_ioremap_attrs,
};

// ---------------------------------------------------------------------
// Power management callbacks (suspend/resume/hibernate)
// ---------------------------------------------------------------------

static int pm_ioremap_suspend(struct device *dev)
{
    struct pm_ioremap_dev *pmdev = dev_get_drvdata(dev);
    u32 ctrl;

    if (!pmdev || !pmdev->base)
        return -ENODEV;

    ctrl = pm_ioremap_readl(pmdev, PM_CTRL_OFFSET);
    pmdev->saved_ctrl = ctrl;

    dev_info(dev, "%s: suspend() - saving PM_CTRL=0x%08x\n",
             DRV_NAME, ctrl);

    // For demo, we ensure deep sleep is enabled before suspend.
    ctrl |= PM_CTRL_DEEP_SLEEP_EN;
    pm_ioremap_writel(pmdev, PM_CTRL_OFFSET, ctrl);

    dev_info(dev, "%s: suspend() - programmed PM_CTRL=0x%08x\n",
             DRV_NAME, ctrl);

    return 0;
}

static int pm_ioremap_resume(struct device *dev)
{
    struct pm_ioremap_dev *pmdev = dev_get_drvdata(dev);
    u32 ctrl;

    if (!pmdev || !pmdev->base)
        return -ENODEV;

    // On resume, restore the previous control value
    ctrl = pmdev->saved_ctrl;

    pm_ioremap_writel(pmdev, PM_CTRL_OFFSET, ctrl);

    dev_info(dev, "%s: resume() - restoring PM_CTRL=0x%08x\n",
             DRV_NAME, ctrl);

    return 0;
}

static int pm_ioremap_freeze(struct device *dev)
{
    dev_info(dev, "%s: freeze() (hibernation) called\n", DRV_NAME);
    return pm_ioremap_suspend(dev);
}

static int pm_ioremap_thaw(struct device *dev)
{
    dev_info(dev, "%s: thaw() called\n", DRV_NAME);
    return pm_ioremap_resume(dev);
}

static int pm_ioremap_poweroff(struct device *dev)
{
    dev_info(dev, "%s: poweroff() called\n", DRV_NAME);
    return 0;
}

static int pm_ioremap_restore(struct device *dev)
{
    dev_info(dev, "%s: restore() (from hibernation) called\n", DRV_NAME);
    return pm_ioremap_resume(dev);
}

static const struct dev_pm_ops pm_ioremap_pm_ops = {
    .suspend  = pm_ioremap_suspend,
    .resume   = pm_ioremap_resume,
    .freeze   = pm_ioremap_freeze,
    .thaw     = pm_ioremap_thaw,
    .poweroff = pm_ioremap_poweroff,
    .restore  = pm_ioremap_restore,
};

// ---------------------------------------------------------------------
// Device tree matching (optional, for DT-based systems / QEMU, etc.)
// ---------------------------------------------------------------------

static const struct of_device_id pm_ioremap_of_match[] = {
    { .compatible = "techdhaba,pm-ioremap-demo", },
    { /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, pm_ioremap_of_match);

// ---------------------------------------------------------------------
// Probe / remove
// ---------------------------------------------------------------------

static int pm_ioremap_probe(struct platform_device *pdev)
{
    struct pm_ioremap_dev *pmdev;
    struct resource *res;
    int ret;
    u32 ctrl;

    dev_info(&pdev->dev, "%s: probe() for device '%s'\n",
             DRV_NAME, dev_name(&pdev->dev));

    pmdev = devm_kzalloc(&pdev->dev, sizeof(*pmdev), GFP_KERNEL);
    if (!pmdev)
        return -ENOMEM;

    // Get MMIO resource from DT/ACPI/board file
    res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
    if (!res) {
        dev_err(&pdev->dev, "%s: no IORESOURCE_MEM found\n", DRV_NAME);
        return -ENODEV;
    }

    // Map it with devm_ioremap_resource (does request_mem_region + ioremap)
    pmdev->base = devm_ioremap_resource(&pdev->dev, res);
    if (IS_ERR(pmdev->base)) {
        dev_err(&pdev->dev, "%s: devm_ioremap_resource failed\n", DRV_NAME);
        return PTR_ERR(pmdev->base);
    }

    // Save driver data pointer
    platform_set_drvdata(pdev, pmdev);

    // Optional: create sysfs group
    ret = sysfs_create_group(&pdev->dev.kobj, &pm_ioremap_attr_group);
    if (ret) {
        dev_err(&pdev->dev, "%s: failed to create sysfs group\n", DRV_NAME);
        return ret;
    }

    // Initialize control register for demo: enable WAKEUP_EN only
    ctrl = pm_ioremap_readl(pmdev, PM_CTRL_OFFSET);
    ctrl |= PM_CTRL_WAKEUP_EN;
    ctrl &= ~PM_CTRL_DEEP_SLEEP_EN;

    pm_ioremap_writel(pmdev, PM_CTRL_OFFSET, ctrl);
    pmdev->saved_ctrl = ctrl;

    dev_info(&pdev->dev, "%s: mapped PM block @ %pa (size=0x%lx), "
             "initial PM_CTRL=0x%08x\n",
             DRV_NAME, &res->start, (unsigned long)resource_size(res), ctrl);

    return 0;
}

static int pm_ioremap_remove(struct platform_device *pdev)
{
    struct pm_ioremap_dev *pmdev = platform_get_drvdata(pdev);

    dev_info(&pdev->dev, "%s: remove() for device '%s'\n",
             DRV_NAME, dev_name(&pdev->dev));

    if (pmdev)
        sysfs_remove_group(&pdev->dev.kobj, &pm_ioremap_attr_group);

    // devm_* will automatically unmap and free memory
    return 0;
}

// ---------------------------------------------------------------------
// Platform driver
// ---------------------------------------------------------------------

static struct platform_driver pm_ioremap_driver = {
    .probe  = pm_ioremap_probe,
    .remove = pm_ioremap_remove,
    .driver = {
        .name           = DRV_NAME,
        .pm             = &pm_ioremap_pm_ops,
        .of_match_table = pm_ioremap_of_match,
    },
};

// Optional: dummy device for non-DT x86 testing
static struct platform_device *pm_ioremap_pdev;

static int __init pm_ioremap_init(void)
{
    int ret;

    pr_info("%s: init\n", DRV_NAME);

    ret = platform_driver_register(&pm_ioremap_driver);
    if (ret) {
        pr_err("%s: platform_driver_register failed: %d\n", DRV_NAME, ret);
        return ret;
    }

    // For pure x86 test without DT/ACPI, create a dummy device
    // using a fake MMIO range (example: 0xFEA00000..0xFEA00FFF).
    // DO NOT use real addresses on a production system.
    {
        struct resource res = {
            .start = 0xFEA00000,
            .end   = 0xFEA00FFF,
            .flags = IORESOURCE_MEM,
        };

        pm_ioremap_pdev = platform_device_register_simple(DRV_NAME, -1,
                                                          &res, 1);
        if (IS_ERR(pm_ioremap_pdev)) {
            pr_err("%s: platform_device_register_simple failed\n", DRV_NAME);
            ret = PTR_ERR(pm_ioremap_pdev);
            platform_driver_unregister(&pm_ioremap_driver);
            return ret;
        }
    }

    pr_info("%s: driver and dummy device registered\n", DRV_NAME);
    return 0;
}

static void __exit pm_ioremap_exit(void)
{
    pr_info("%s: exit\n", DRV_NAME);

    if (pm_ioremap_pdev && !IS_ERR(pm_ioremap_pdev))
        platform_device_unregister(pm_ioremap_pdev);

    platform_driver_unregister(&pm_ioremap_driver);
}

module_init(pm_ioremap_init);
module_exit(pm_ioremap_exit);

MODULE_AUTHOR("TechDhaba");
MODULE_DESCRIPTION("x86/i5-friendly power management demo using ioremap");
MODULE_LICENSE("GPL");
