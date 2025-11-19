#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main() {
    int fd;
    char write_buf[] = "Hello from system calls!\n";
    char read_buf[100];
    ssize_t n;

    // open file
    fd = open("syscall_file.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    // write to file
    if (write(fd, write_buf, strlen(write_buf)) < 0) {
        perror("write");
        close(fd);
        return 1;
    }
    close(fd);

    // reopen for reading
    fd = open("syscall_file.txt", O_RDONLY);
    if (fd < 0) {
        perror("open read");
        return 1;
    }

    // read from file
    n = read(fd, read_buf, sizeof(read_buf) - 1);
    if (n < 0) {
        perror("read");
        close(fd);
        return 1;
    }
    read_buf[n] = '\0';

    // print data
    printf("Read using system calls:\n%s", read_buf);

    close(fd);
    return 0;
}

