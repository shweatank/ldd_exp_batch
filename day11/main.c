#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/io.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/of.h>

// UART Regs 
#define UART_BASE 0xFE201000 /* Change to 0x3F201000 for Pi 3 */
#define UART_DR   0x00  /* Data Register */
#define UART_FR   0x18  /* Flag Register */
#define UART_IBRD 0x24  /* Integer Baud Rate Divisor */
#define UART_FBRD 0x28  /* Fractional Baud Rate Divisor */
#define UART_LCRH 0x2C  /* Line Control Register */
#define UART_CR   0x30  /* Control Register */
#define UART_IMSC 0x38  /* Interrupt Mask Set/Clear Register */
#define UART_ICR  0x44  /* Interrupt Clear Register */

/* Flag Register Bits */
#define UART_FR_TXFF (1 << 5) /* Transmit FIFO Full */
#define UART_FR_RXFE (1 << 4) /* Receive FIFO Empty */
#define UART_FR_BUSY (1 << 3) /* UART Busy */

/* Line Control Register Bits */
#define UART_LCRH_WLEN_8BIT (0x3 << 5) /* 8-bit word length */
#define UART_LCRH_FEN       (1 << 4)   /* Enable FIFOs */

/* Control Register Bits */
#define UART_CR_UARTEN (1 << 0)  /* UART Enable */
#define UART_CR_TXE    (1 << 8)  /* Transmit Enable */
#define UART_CR_RXE    (1 << 9)  /* Receive Enable */

/* Device parameters */
#define DRIVER_NAME "rpi_uart"
#define DEVICE_NAME "rpi_uart"
#define BUFFER_SIZE 1024

/* Module structure */
struct rpi_uart_dev {
        struct cdev cdev;
        void __iomem regs; /* Mapped UART registers */
                dev_t dev_no;
        struct class *class;
        struct device *device;
};

static struct rpi_uart_dev *uart_dev;

/* UART register access macros */
#define uart_readl(offset)      readl(uart_dev->regs + offset)
#define uart_writel(val, offset) writel(val, uart_dev->regs + offset)

/* Device tree matching */
static const struct of_device_id rpi_uart_of_match[] = {
	{ .compatible = "rpi,uart-driver" },
	{ /* Sentinel */ }
};
MODULE_DEVICE_TABLE(of, rpi_uart_of_match);

/* Platform driver structure */
static struct platform_driver rpi_uart_driver = {
	.probe = rpi_uart_probe,
	.remove = rpi_uart_remove,
	.driver = {
		.name = DRIVER_NAME,
		.of_match_table = rpi_uart_of_match,
	},
};

static int __init comm_init(void){
		
    return platform_driver_register(&rpi_uart_driver);

}

static void __exit comm_exit(void){
	
}

module_init(comm_init);
module_exit(comm_exit);

MODULE_LICENSE("GPL");

