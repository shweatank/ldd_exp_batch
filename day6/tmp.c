#include <stdio.h>
#include <fcntl.h>      // open()
#include <unistd.h>     // read(), write(), close()
#include <string.h>     // strlen()

int main() {
    int fd;
    char buffer[100];
    char *text = "Hello, this is a test using system calls!\n";

    // 1. Open the file (create if it doesn't exist)
    fd = open("test.txt", O_CREAT | O_RDWR | O_TRUNC, 0644);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    // 2. Write to the file
    if (write(fd, text, strlen(text)) < 0) {
        perror("write");
        close(fd);
        return 1;
    }

    // Move file offset back to beginning for reading
    lseek(fd, 0, SEEK_SET);

    // 3. Read from the file
    int bytes = read(fd, buffer, sizeof(buffer) - 1);
    if (bytes < 0) {
        perror("read");
        close(fd);
        return 1;
    }

    buffer[bytes] = '\0';     // Null-terminate for printing

    // 4. Print the content
    printf("File content: %s", buffer);

    close(fd);
    return 0;
}

