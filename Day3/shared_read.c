
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#define SHM_NAME "/my_shared_mem"
#define SHM_SIZE 1024

int main(void) {
    // 1. Open existing shared memory
    int fd = shm_open(SHM_NAME, O_RDONLY, 0666);
    if (fd == -1) {
        perror("shm_open");
        return EXIT_FAILURE;
    }

    // 2. Map shared memory into process space
    void *ptr = mmap(0, SHM_SIZE, PROT_READ, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap");
        return EXIT_FAILURE;
    }

    // 3. Read and display message
    printf("Reader: message read -> \"%s\"\n", (char *)ptr);

    // 4. Clean up
    munmap(ptr, SHM_SIZE);
    close(fd);

    // Optional: remove shared memory
    shm_unlink(SHM_NAME);

    return EXIT_SUCCESS;
}
