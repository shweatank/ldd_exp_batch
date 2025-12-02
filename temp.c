#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>

int open_and_configure_port(const char *port_path, speed_t baud_rate)
{
    int fd = open(port_path, O_RDWR | O_NOCTTY | O_NDELAY);

    if (fd < 0)
    {
        perror("Error opening serial port");
        return -1;
    }

    struct termios tty;

    if (tcgetattr(fd, &tty) != 0)
    {
        perror("Error from tcgetattr");
        close(fd);
        return -1;
    }

    tty.c_cflag &= ~PARENB;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;
    tty.c_cflag &= ~CRTSCTS;
    tty.c_cflag |= CREAD | CLOCAL;

    tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

    tty.c_iflag &= ~(IXON | IXOFF | IXANY);
    tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL);

    tty.c_oflag &= ~OPOST;

    cfsetospeed(&tty, baud_rate);
    cfsetispeed(&tty, baud_rate);

    if (tcsetattr(fd, TCSANOW, &tty) != 0)
    {
        perror("Error from tcsetattr");
        close(fd);
        return -1;
    }

    return fd;
}

int main()
{
    const char *port_name = "/dev/serial0";
    speed_t baud_rate = B9600;
    char data_to_send[] = "Hello RPi 2, this is RPi 1 over UART!\n";
    int fd;

    printf("Starting UART Sender...\n");

    fd = open_and_configure_port(port_name, baud_rate);
    if (fd < 0)
    {
        fprintf(stderr, "Failed to initialize UART. Check wiring and 'raspi-config' settings.\n");
        return 1;
    }

    printf("Serial port opened successfully at 9600 baud. Sending data...\n");

    while (1)
    {
        int bytes_written = write(fd, data_to_send, strlen(data_to_send));

        if (bytes_written < 0)
        {
            perror("Error writing to port");
        }
        else
        {
            printf("Sent %d bytes: %s", bytes_written, data_to_send);
        }

        tcdrain(fd);
        sleep(1);
    }

    return 0;
}