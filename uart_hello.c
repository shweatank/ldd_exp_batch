#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/io.h>

#define UART0_BASE 0xFE201000 // PL011 base for RPi 4
#define UART_FR 0x18          // Flag Register
#define UART_DR 0x00          // Data Register
#define UART_CR 0x30          // Control Register
#define UART_IBRD 0x24        // Integer Baud Rate
#define UART_FBRD 0x28        // Fractional Baud Rate
#define UART_LCRH 0x2C        // Line Control
#define UART_IMSC 0x38        // Interrupt Mask

static void __iomem *uart_base;

static void uart_send_char(char c)
{
    // Wait until TX FIFO is not full
    while (readl(uart_base + UART_FR) & (1 << 5))
        ;
    writel(c, uart_base + UART_DR);
}

static void uart_send_string(const char *s)
{
    while (*s)
    {
        uart_send_char(*s++);
    }
}

static int __init uart_init(void)
{
    pr_info("UART Hello Driver Init\n");

    uart_base = ioremap(UART0_BASE, 0x100);
    if (!uart_base)
    {
        pr_err("UART ioremap failed\n");
        return -ENOMEM;
    }

    // Disable UART before configuring
    writel(0x0, uart_base + UART_CR);

    // Set baud rate (48MHz / (16 * 26.1875)) = 115200
    writel(26, uart_base + UART_IBRD);
    writel(3, uart_base + UART_FBRD);

    // 8N1, enable FIFO
    writel((3 << 5) | (1 << 4), uart_base + UART_LCRH);

    // Disable all UART interrupts
    writel(0x0, uart_base + UART_IMSC);

    // Enable UART, TX and RX
    writel((1 << 0) | (1 << 8) | (1 << 9), uart_base + UART_CR);

    // Send "Hello"
    uart_send_string("Hello\n");

    return 0;
}

static void __exit uart_exit(void)
{
    writel(0x0, uart_base + UART_CR); // Disable UART
    iounmap(uart_base);
    pr_info("UART Hello Driver Exit\n");
}

module_init(uart_init);
module_exit(uart_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("techdhaba");
MODULE_DESCRIPTION("Minimal UART driver that sends Hello over GPIO14 TX");
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define SERIAL_PORT "/dev/rpi_uart"
 
int main()
{
    int fd;
    char buf[256];
    ssize_t n;
    const char *init_msg = "Hello from Raspi\n";
    const char *response = "go\n";

    /* Open serial port */
    fd = open(SERIAL_PORT, O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd < 0)
    {
        perror("Failed to open serial port");
        exit(1);
    }

    /* Set non-blocking read */
    if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0)
    {
        perror("Failed to set non-blocking");
        close(fd);
        exit(1);
    }

    printf("Serial port opened. Sending initial message...\n");

    /* Send initial message */
    n = write(fd, init_msg, strlen(init_msg));
    if (n < 0)
    {
        perror("Write error");
        close(fd);
        exit(1);
    }
    printf("Sent: %s", init_msg);

    /* Main loop */
    while (1)
    {
        /* Read from serial port */
        n = read(fd, buf, sizeof(buf) - 1);
        if (n > 0)
        {
            buf[n] = '\0';
            printf("Received: %s", buf);
            /* Check for "hi" */
            if (strstr(buf, "hi") != NULL)
            {
                n = write(fd, response, strlen(response));
                if (n < 0)
                {
                    perror("Write error");
                    close(fd);
                    exit(1);
                }
                printf("Sent: %s", response);
            }
        }
        else if (n < 0 && errno != EAGAIN && errno != EWOULDBLOCK)
        {
            perror("Read error");
            close(fd);
            exit(1);
        }

        /* Send periodic message every 5 seconds */
        sleep(5);
        n = write(fd, init_msg, strlen(init_msg));
        if (n < 0)
        {
            perror("Write error");
            close(fd);
            exit(1);
        }
        printf("Sent: %s", init_msg);
    }

    close(fd);
    return 0;
}