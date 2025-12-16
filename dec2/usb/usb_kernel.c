#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>

/* --------- Change these to match your USB device --------- */
#define MY_USB_VENDOR_ID   0x1234  /* example, replace with real VID */
#define MY_USB_PRODUCT_ID  0x5678  /* example, replace with real PID */
/* --------------------------------------------------------- */

/* Table of devices this driver supports */
static const struct usb_device_id my_usb_table[] = {
    { USB_DEVICE(MY_USB_VENDOR_ID, MY_USB_PRODUCT_ID) },
    { } /* Terminating entry */
};
MODULE_DEVICE_TABLE(usb, my_usb_table);

/*
 * probe() is called by the USB core when:
 *  - A device with matching VID:PID is plugged in, OR
 *  - The driver is loaded while such a device is already present.
 */
static int my_usb_probe(struct usb_interface *interface,
                        const struct usb_device_id *id)
{
    struct usb_device *udev = interface_to_usbdev(interface);
    struct usb_host_interface *iface_desc;
    struct usb_endpoint_descriptor *endpoint;
    int i;

    dev_info(&interface->dev,
             "my_usb_basic_pc: USB device plugged in (VID=0x%04x PID=0x%04x)\n",
             le16_to_cpu(udev->descriptor.idVendor),
             le16_to_cpu(udev->descriptor.idProduct));

    /* Current alternate setting (interface descriptor) */
    iface_desc = interface->cur_altsetting;

    dev_info(&interface->dev,
             "my_usb_basic_pc: Interface %d, %d endpoints\n",
             iface_desc->desc.bInterfaceNumber,
             iface_desc->desc.bNumEndpoints);

    /* Print basic info about each endpoint */
    for (i = 0; i < iface_desc->desc.bNumEndpoints; i++) {
        endpoint = &iface_desc->endpoint[i].desc;

        dev_info(&interface->dev,
                 "  Endpoint %d:\n"
                 "    bEndpointAddress: 0x%02x\n"
                 "    bmAttributes    : 0x%02x\n"
                 "    wMaxPacketSize  : %d\n",
                 i,
                 endpoint->bEndpointAddress,
                 endpoint->bmAttributes,
                 le16_to_cpu(endpoint->wMaxPacketSize));
    }

    dev_info(&interface->dev,
             "my_usb_basic_pc: probe() successful, driver bound.\n");

    /*
     * For a real driver:
     *   - Allocate a per-device struct
     *   - Save pointers (udev, interface, pipes, etc.)
     *   - usb_set_intfdata(interface, data);
     */

    return 0; /* success */
}

/*
 * disconnect() is called when:
 *  - The device is unplugged, OR
 *  - The driver is unloaded while the device is present.
 */
static void my_usb_disconnect(struct usb_interface *interface)
{
    dev_info(&interface->dev,
             "my_usb_basic_pc: USB device disconnected\n");

    /*
     * If you had allocated per-interface data:
     *
     *   struct my_usb_data *data = usb_get_intfdata(interface);
     *   usb_set_intfdata(interface, NULL);
     *   kfree(data);
     */
}

/* Our usb_driver structure */
static struct usb_driver my_usb_driver = {
    .name       = "my_usb_basic_pc",
    .id_table   = my_usb_table,
    .probe      = my_usb_probe,
    .disconnect = my_usb_disconnect,
};

static int __init my_usb_init(void)
{
    int ret;

    pr_info("my_usb_basic_pc: init\n");

    ret = usb_register(&my_usb_driver);
    if (ret) {
        pr_err("my_usb_basic_pc: usb_register failed, error %d\n", ret);
        return ret;
    }

    pr_info("my_usb_basic_pc: registered with USB core\n");
    return 0;
}

static void __exit my_usb_exit(void)
{
    pr_info("my_usb_basic_pc: exit\n");
    usb_deregister(&my_usb_driver);
}

module_init(my_usb_init);
module_exit(my_usb_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("TechDhaba");
MODULE_DESCRIPTION("Basic example USB driver for x86 Linux");
MODULE_VERSION("1.0");
