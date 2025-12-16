#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#define DEVICE "/dev/my_uart0"
#define BUF_SIZE 256

int main() {
    int fd = open(DEVICE, O_RDWR | O_NONBLOCK);
    if (fd < 0) {
        perror("Failed to open device");
        return 1;
    }

    printf("Opened %s\n", DEVICE);

    char write_buf[] = "Hello from userspace!\n";
    char read_buf[BUF_SIZE];
    int counter = 0;

    while (1) {
        // -------- WRITE --------
        int w = write(fd, write_buf, strlen(write_buf));
        if (w < 0) {
            perror("write");
        } else {
            printf("[WRITE] %d bytes: %s", w, write_buf);
        }

        // -------- READ --------
	printf("waiting for read\n");

        int r = read(fd, read_buf, BUF_SIZE - 1);
        if (r > 0) {
            read_buf[r] = '\0';
            printf("[READ] %d bytes: %s\n", r, read_buf);
        } else if (r < 0 && errno != EAGAIN) {
            perror("read");
        }

        // Sleep to avoid spamming
        usleep(500 * 1000); // 500ms
    }

    close(fd);
    return 0;
}

