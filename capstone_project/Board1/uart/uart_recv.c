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

	char ch;
    while (1) {
        n = read(fd, &ch, 1);
        if (n > 0) {
			printf("%c\n", ch);
        }

    }

    close(fd);
    return 0;
}

