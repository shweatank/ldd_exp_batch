#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main() {
    int fd;
    char write_buf[] = "Hello Linux System Calls!";
    char read_buf[100];
    ssize_t bytes;

   
    fd = open("myfile.txt", O_CREAT | O_RDWR, 0644);
    if (fd == -1) {
        perror("open");
        return 1;
    }
    printf("File opened successfully, fd = %d\n", fd);

    bytes = write(fd, write_buf, strlen(write_buf));
    if (bytes == -1) {
        perror("write");
        close(fd);
        return 1;
    }
    printf("Written %zd bytes\n", bytes);

    lseek(fd, 0, SEEK_SET);

    bytes = read(fd, read_buf, sizeof(read_buf) - 1);
    if (bytes == -1) {
        perror("read");
        close(fd);
        return 1;
    }
    read_buf[bytes] = '\0';  
    printf("Read %zd bytes: %s\n", bytes, read_buf);

  
    if (close(fd) == -1) {
        perror("close");
        return 1;
    }

    printf("File closed successfully.\n");
    return 0;
}

