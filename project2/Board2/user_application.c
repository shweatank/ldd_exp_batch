#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <termios.h>

/* IOCTL */
#define IR_MAGIC        'i'
#define IR_GET_STATUS   _IOR(IR_MAGIC, 1, int)

#define UART_DEV "/dev/my_uart0"   // change if needed

int main(void)
{
    int ir_fd, uart_fd;
    int status;
    struct termios uart_cfg;
    const char *msg = "Object detected\n";

    /* Open IR sensor device */
    ir_fd = open("/dev/ir_sensor", O_RDWR);
    if (ir_fd < 0) {
        perror("IR open");
        return 1;
    }

    /* Open UART */
    uart_fd = open(UART_DEV, O_RDWR | O_NOCTTY);
    if (uart_fd < 0) {
        perror("UART open");
        close(ir_fd);
        return 1;
    }

    printf("Monitoring IR sensor...\n");

    while (1) {
        ioctl(ir_fd, IR_GET_STATUS, &status);

        if (status == 1) {
            printf("Object detected! Sending to UART...\n");
            write(uart_fd, msg, strlen(msg));
        }

    }

    close(uart_fd);
    close(ir_fd);
    return 0;
}

