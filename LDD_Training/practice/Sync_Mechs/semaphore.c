#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>

int count = 0;
sem_t sem;

void *inc(void *data) {
    // 10 million iterations to ensure high CPU load
    for (int i = 0; i < 10000000; i++) {
        sem_wait(&sem);    // Wait/Down (Counter goes 1 -> 0)
        count++;           // CRITICAL SECTION (Protected)
        sem_post(&sem);    // Post/Up (Counter goes 0 -> 1)
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t Th1, Th2;

    // STEP 1: Initialize semaphore BEFORE creating threads
    // Parameters: pointer, pshared (0 for threads), initial value (1)
    if (sem_init(&sem, 0, 1) != 0) {
        perror("Semaphore init failed");
        return 1;
    }

    // STEP 2: Create threads
    pthread_create(&Th1, NULL, inc, NULL);
    pthread_create(&Th2, NULL, inc, NULL);

    // STEP 3: Wait for threads to finish
    pthread_join(Th1, NULL);
    pthread_join(Th2, NULL);

    // STEP 4: Check result
    printf("Final count = %d\n", count);
    if(count == 20000000) {
        printf("SUCCESS: Synchronization working correctly!\n");
    } else {
        printf("SYNC Issue - count is %d\n", count);
    }

    // STEP 5: Clean up
    sem_destroy(&sem);

    return 0;
}
