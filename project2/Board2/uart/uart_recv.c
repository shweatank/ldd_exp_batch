#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define SERIAL_PORT "/dev/my_uart0"

int main() {
    int fd;
    char buf[256];
    char line[256];
    ssize_t n;
    int idx = 0;

    fd = open(SERIAL_PORT, O_RDWR | O_NOCTTY);  // blocking read
    if (fd < 0) { perror("open"); exit(1); }

    printf("Serial port opened\n");

    while (1) {
        n = read(fd, buf, sizeof(buf));
        if (n < 0) {
            perror("read");
            break;
        }

        for (int i = 0; i < n; i++) {
            line[idx++] = buf[i];
            if (buf[i] == '\n' || idx == sizeof(line)-1) {
                line[idx] = '\0';
                printf("Received: %s", line);  // full line
                idx = 0;
            }
        }
    }

    close(fd);
    return 0;
}

