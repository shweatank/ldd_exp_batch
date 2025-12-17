#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/serial_reg.h>
#include <linux/platform_device.h>
#include <linux/interrupt.h>
#include <linux/delay.h>

#define UART0_BASE 0x3F201000  // Base address of UART0 for BCM2711
#define UART0_CLK_SPEED 50000000  // 50 MHz clock for UART0 (default)

#define UART_REG_DR 0x00  // Data Register
#define UART_REG_FR 0x18  // Flag Register
#define UART_REG_IBRD 0x24  // Integer Baud Rate Divisor
#define UART_REG_FBRD 0x28  // Fractional Baud Rate Divisor
#define UART_REG_LCRH 0x2C  // Line Control Register
#define UART_REG_CR 0x30  // Control Register

#define UART_FR_TXFF (1 << 5)  // Transmit FIFO full flag
#define UART_FR_RXFE (1 << 4)  // Receive FIFO empty flag

#define BAUD_RATE 115200

static void __iomem *uart_base;
static struct platform_device *uart_device;

static void uart_putc(char c)
{
    // Wait until the transmit FIFO is not full
    while (readl(uart_base + UART_REG_FR) & UART_FR_TXFF)
        ;

    // Write the character to the data register
    writel(c, uart_base + UART_REG_DR);
}

static char uart_getc(void)
{
    // Wait until the receive FIFO is not empty
    while (readl(uart_base + UART_REG_FR) & UART_FR_RXFE)
        ;

    // Read the character from the data register
    return readl(uart_base + UART_REG_DR) & 0xFF;
}

static int uart_probe(struct platform_device *pdev)
{
    unsigned int baud_divisor, ibrd, fbrd;

    // Map UART0 registers
    uart_base = ioremap(UART0_BASE, 0x1000);
    if (!uart_base) {
        pr_err("Failed to map UART0 registers\n");
        return -ENOMEM;
    }

    // Set baud rate (115200)
    baud_divisor = UART0_CLK_SPEED / (16 * BAUD_RATE);
    ibrd = baud_divisor >> 6;    // Integer part
    fbrd = baud_divisor & 0x3F;  // Fractional part

    // Set baud rate in IBRD and FBRD
    writel(ibrd, uart_base + UART_REG_IBRD);
    writel(fbrd, uart_base + UART_REG_FBRD);

    // Configure UART: 8N1 (8 data bits, no parity, 1 stop bit)
    writel(0x60, uart_base + UART_REG_LCRH);  // 0x60 = 01100000
    writel(0x301, uart_base + UART_REG_CR);   // 0x301 = Enable UART, TX, RX

    pr_info("UART driver initialized with probe function\n");
    return 0;
}

static int uart_remove(struct platform_device *pdev)
{
    // Cleanup: unmap UART0 registers
    iounmap(uart_base);
    pr_info("UART driver removed\n");
    return 0;
}

static struct platform_driver uart_platform_driver = {
    .probe = uart_probe,
    .remove = uart_remove,
    .driver = {
        .name = "bcms2711-uart",
        .owner = THIS_MODULE,
    },
};

static int __init uart_driver_init(void)
{
    int ret;

    // Register the platform driver
    ret = platform_driver_register(&uart_platform_driver);
    if (ret) {
        pr_err("Failed to register UART platform driver\n");
        return ret;
    }

    pr_info("UART driver registered successfully\n");
    return 0;
}

static void __exit uart_driver_exit(void)
{
    // Unregister the platform driver
    platform_driver_unregister(&uart_platform_driver);
    pr_info("UART driver unregistered\n");
}

module_init(uart_driver_init);
module_exit(uart_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A simple UART driver for BCM2711 (Raspberry Pi 4) using probe()");
:se nu

