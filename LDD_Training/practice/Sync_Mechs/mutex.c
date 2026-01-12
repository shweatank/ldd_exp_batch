#include <stdio.h>
#include <pthread.h>

int count = 0;
pthread_mutex_t mutex;
void *inc(void *data) {
	for (int i=0; i<10000000; i++) {
		pthread_mutex_lock(&mutex);
		count++;
		pthread_mutex_unlock(&mutex);
	}
	pthread_exit(NULL);
}

int main() {
	pthread_t Th1, Th2;

	pthread_create(&Th1, NULL, inc, NULL);
	pthread_create(&Th2, NULL, inc, NULL);

	pthread_mutex_init(&mutex, NULL);

	pthread_join(Th1, NULL);
	pthread_join(Th2, NULL);

	printf("count = %d\n", count);
	if(count != 20000000) printf("SYNC Issued\n");

	pthread_mutex_destroy(&mutex);

	return 0;
}

