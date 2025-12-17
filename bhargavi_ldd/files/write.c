#include <stdio.h>
#include <unistd.h>      // for write(), close()
#include <fcntl.h>       // for open()
#include <string.h>      // for strlen()

int main() {
    int fd;
    char data[] = "Hello! This is written using the write() system call.\n";

    // 1. Open the file
    fd = open("file.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        printf("Error opening the file!\n");
        return 1;
    }

    // 2. Write to the file
    write(fd, data, strlen(data));

    // 3. Close the file
    close(fd);

    printf("Data written successfully using write()!\n");

    return 0;
}

