#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main() {
    // Generate same key as writer
    key_t key = ftok("shmfile", 65);

    // Get the shared memory segment
    int shmid = shmget(key, 1024, 0666);

    // Attach to shared memory
    char *str = (char *)shmat(shmid, NULL, 0);

    // Read and print the data
    printf("Reader read: %s\n", str);

    // Detach from shared memory
    shmdt(str);

    // Destroy the shared memory
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}
