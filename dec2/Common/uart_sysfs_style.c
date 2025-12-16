//uart_sysfs_style.c — UART TX/RX Program (Similar to LED Sysfs Code)
// uart_sysfs_style.c
// UART open, configure, write, read for Raspberry Pi 4B
// Compile: gcc uart_sysfs_style.c -o uart
// Run: sudo ./uart
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
 
#define UART_DEV "/dev/ttyAMA0"     // Use PL011 UART
#define BAUDRATE B115200
 
int uart_configure(int fd)
{
    struct termios tty;
 
    if (tcgetattr(fd, &tty) != 0) {
        perror("tcgetattr");
        return -1;
    }
 
    cfmakeraw(&tty);
 
    cfsetispeed(&tty, BAUDRATE);
    cfsetospeed(&tty, BAUDRATE);
 
    tty.c_cflag |= (CLOCAL | CREAD);   // Enable receiver
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;                // 8-bit chars
    tty.c_cflag &= ~PARENB;            // No parity
    tty.c_cflag &= ~CSTOPB;            // 1 stop bit
    tty.c_cflag &= ~CRTSCTS;           // No flow control
 
    tty.c_cc[VMIN] = 0;
    tty.c_cc[VTIME] = 10;              // Timeout 1 sec
 
    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        perror("tcsetattr");
        return -1;
    }
 
    return 0;
}
 
int uart_write(int fd, const char *msg)
{
    int len = strlen(msg);
    int n = write(fd, msg, len);
    
    if (n < 0) {
        perror("UART write failed");
        return -1;
    }
 
    return n;
}
 
int uart_read(int fd, char *buf, int size)
{
    int n = read(fd, buf, size - 1);
    if (n < 0) {
        perror("UART read failed");
        return -1;
    }
 
    buf[n] = '\0';
    return n;
}
 
int main()
{
    printf("Opening UART: %s\n", UART_DEV);
 
    int fd = open(UART_DEV, O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd < 0) {
        perror("Failed to open UART");
        return 1;
    }
 
    if (uart_configure(fd) < 0) {
        close(fd);
        return 1;
    }
 
    printf("UART configured successfully at 115200 8N1\n");
 
    // Write example
    const char *msg = "Hello from Raspberry Pi UART!\r\n";
    uart_write(fd, msg);
    printf("TX: %s", msg);
 
    // Read example
    char buffer[128];
	while(1) {
    int n = uart_read(fd, buffer, sizeof(buffer));
    if (n > 0) {
        printf("RX: %s\n", buffer);
    }
 	}
    close(fd);
    return 0;
}
