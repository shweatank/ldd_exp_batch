#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define DEVICE_PATH "/dev/simple_char_dev"
#define BUF_SIZE 256

int main() {
    int fd;
 //   char write_buf[] = "Hello from user space!";
    char read_buf[BUF_SIZE];
    ssize_t ret;

    /* Open device */
    fd = open(DEVICE_PATH, O_RDWR);
    if (fd < 0) {
        perror("Failed to open device");
        return 1;
    }
    printf("Device opened successfully.\n");

   /* //Write to device 
    ret = write(fd, write_buf, strlen(write_buf));
    if (ret < 0) {
        perror("Write failed");
        close(fd);
        return 1;
    }
    printf("Wrote %ld bytes: \"%s\"\n", ret, write_buf);

    // Reset file offset before reading 
    lseek(fd, 0, SEEK_SET);
*/
    /* Read from device */
    memset(read_buf, 0, BUF_SIZE);
    ret = read(fd, read_buf, BUF_SIZE);
    if (ret < 0) {
        perror("Read failed");
        close(fd);
        return 1;
    }
    printf("Read %ld bytes: \"%s\"\n", ret, read_buf);

    /* Close device */
    close(fd);
    printf("Device closed.\n");

    return 0;
}

