#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>

int main() {
    // Generate unique key
    key_t key = ftok("shmfile", 65);

    // Create shared memory segment of size 1024 bytes
    int shmid = shmget(key, 1024, 0666 | IPC_CREAT);

    // Attach the shared memory segment
    char *str = (char *)shmat(shmid, NULL, 0);

    // Write data to shared memory
    strcpy(str, "Hello from writer!");

    printf("Writer wrote: %s\n", str);

    // Detach from shared memory
    shmdt(str);

    return 0;
}
