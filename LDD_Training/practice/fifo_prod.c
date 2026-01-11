#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

int main() {
    const char *path = "./myfifo"; // Local path

    // 1. Create the FIFO in the current directory
    mkfifo(path, 0666);
    
    // 2. Open for WRITING only
    // This will block here until the Consumer opens it for reading
    int fd = open(path, O_WRONLY);

    write(fd, "Hello...", 9);
    printf("Fifo sent Hello...\n");

    close(fd);
    return 0;
}
