#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

int main() {
    const char *path = "./myfifo";

    // 1. Open for READING only
    int fd = open(path, O_RDONLY);
    
    char str[10] = {0}; // Initialize with zeros to handle null-termination
    read(fd, str, 9);
    
    printf("Fifo receives = %s\n", str);

    close(fd);
    // Cleanup: Remove the FIFO file from the disk
    unlink(path); 
    return 0;
}
