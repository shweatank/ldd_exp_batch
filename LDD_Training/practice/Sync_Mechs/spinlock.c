#include <stdio.h>
#include <pthread.h>

int count = 0;
pthread_spinlock_t spinlock;

void *inc(void *data) {
    for (int i = 0; i < 10000000; i++) {
        pthread_spin_lock(&spinlock);   // Spins until free
        count++; 
        pthread_spin_unlock(&spinlock); // Releases
    }
    return NULL;
}

int main() {
    pthread_t Th1, Th2;

    pthread_spin_init(&spinlock, 0);

    pthread_create(&Th1, NULL, inc, NULL);
    pthread_create(&Th2, NULL, inc, NULL);

    pthread_join(Th1, NULL);
    pthread_join(Th2, NULL);

    printf("Final count: %d\n", count);
    pthread_spin_destroy(&spinlock);
    return 0;
}
