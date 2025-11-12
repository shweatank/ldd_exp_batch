#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>      // For O_* constants
#include <sys/mman.h>   // For shm_open, mmap
#include <sys/stat.h>   // For mode constants
#include <unistd.h>     // For ftruncate, close

#define SHM_NAME "/my_shared_mem"
#define SHM_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <message>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // 1. Create or open shared memory
    int fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (fd == -1) {
        perror("shm_open");
        return EXIT_FAILURE;
    }

    // 2. Resize the shared memory
    if (ftruncate(fd, SHM_SIZE) == -1) {
        perror("ftruncate");
        return EXIT_FAILURE;
    }

    // 3. Map shared memory into process space
    void *ptr = mmap(0, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap");
        return EXIT_FAILURE;
    }

    // 4. Write message into shared memory
    snprintf((char *)ptr, SHM_SIZE, "%s", argv[1]);
    printf("Writer: message written -> \"%s\"\n", (char *)ptr);

    // 5. Clean up
    munmap(ptr, SHM_SIZE);
    close(fd);
    return EXIT_SUCCESS;
}
