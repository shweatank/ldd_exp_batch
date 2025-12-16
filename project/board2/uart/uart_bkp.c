#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/of_address.h>
#include <linux/of_device.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/io.h>

#define DRIVER_NAME "my_uart0"

/* PL011 registers offsets */
#define UART_DR     0x00    // Data register
#define UART_FR     0x18    // Flag register
#define UART_IBRD   0x24
#define UART_FBRD   0x28
#define UART_LCRH   0x2C
#define UART_CR     0x30
#define UART_IMSC   0x38
#define UART_ICR    0x44

/* flag bits */
#define FR_TXFF (1 << 5)
#define FR_RXFE (1 << 4)
#define CR_UARTEN (1 << 0)
#define CR_TXE    (1 << 8)
#define CR_RXE    (1 << 9)

static void __iomem *uart_base;
static int major;

static int uart_open(struct inode *inode, struct file *file)
{
    return 0;
}

static int uart_release(struct inode *inode, struct file *file)
{
    return 0;
}

/* POLLING WRITE */
static ssize_t uart_write(struct file *file, const char __user *buf,
                          size_t len, loff_t *offset)
{
    size_t i;
    char c;

    for (i = 0; i < len; i++) {
        if (copy_from_user(&c, &buf[i], 1))
            return -EFAULT;

        /* Wait until TX FIFO not full */
        while (readl(uart_base + UART_FR) & FR_TXFF)
            cpu_relax();

        writel(c, uart_base + UART_DR);  // write char
    }

    return len;
}

/* POLLING READ */
static ssize_t uart_read(struct file *file, char __user *buf,
                         size_t len, loff_t *offset)
{
    size_t i;
    char c;

    for (i = 0; i < len; i++) {
        /* Wait for RX FIFO not empty */
        while (readl(uart_base + UART_FR) & FR_RXFE)
            cpu_relax();

        c = readl(uart_base + UART_DR);
        if (copy_to_user(&buf[i], &c, 1))
            return -EFAULT;
    }
	
    return len;
}

static struct file_operations uart_fops = {
    .owner = THIS_MODULE,
    .open = uart_open,
    .release = uart_release,
    .read = uart_read,
    .write = uart_write,
};

static int uart_probe(struct platform_device *pdev)
{
    struct resource *res;

    pr_info("my_uart0: probe called\n");

    res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
    uart_base = devm_ioremap_resource(&pdev->dev, res);
    if (IS_ERR(uart_base))
        return PTR_ERR(uart_base);

    /* Disable interrupts */
    writel(0, uart_base + UART_IMSC);

    /* Clear interrupt flags */
    writel(0x7FF, uart_base + UART_ICR);

    /* Disable UART */
    writel(0, uart_base + UART_CR);

    /* Set baud rate to 115200 (default Pi clock settings) */
    writel(26, uart_base + UART_IBRD);
    writel(3, uart_base + UART_FBRD);

    /* 8-bit, FIFO disabled for simplicity */
    writel((3 << 5), uart_base + UART_LCRH);

    /* Enable UART, RX, TX */
    writel(CR_UARTEN | CR_TXE | CR_RXE, uart_base + UART_CR);

    /* Register char device */
    major = register_chrdev(0, DRIVER_NAME, &uart_fops);
    pr_info("my_uart0: registered char device with major %d\n", major);

    return 0;
}

static void uart_remove(struct platform_device *pdev)
{
    unregister_chrdev(major, DRIVER_NAME);
}

static const struct of_device_id uart_dt_ids[] = {
    { .compatible = "mycompany,my-uart0" },  // <--- DT match
    {}
};
MODULE_DEVICE_TABLE(of, uart_dt_ids);

static struct platform_driver uart_driver = {
    .probe = uart_probe,
    .remove = uart_remove,
    .driver = {
        .name = DRIVER_NAME,
        .of_match_table = uart_dt_ids,
    },
};

module_platform_driver(uart_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("You");
MODULE_DESCRIPTION("Polling UART0 driver for Raspberry Pi 4B (BCM2711)");

