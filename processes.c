#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>      
#include <sys/mman.h>   
#include <sys/stat.h>   
#include <unistd.h>     

#define SHM_NAME "shared_mem"
#define SHM_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <message>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (fd == -1) {
        perror("shm_open");
        return EXIT_FAILURE;
    }

    if (ftruncate(fd, SHM_SIZE) == -1) {
        perror("ftruncate");
        return EXIT_FAILURE;
    }

    void *ptr = mmap(0, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap");
        return EXIT_FAILURE;
    }

    snprintf((char *)ptr, SHM_SIZE, "%s", argv[1]);
    printf("Writer: message written -> \"%s\"\n", (char *)ptr);

    munmap(ptr, SHM_SIZE);
    close(fd);
    return EXIT_SUCCESS;
}