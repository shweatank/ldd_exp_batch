#include <stdio.h>
#include <pthread.h>

int count = 0;
void *inc(void *data) {
	for (int i=0; i<10000000; i++) count++;

	pthread_exit(NULL);
}

int main() {
	pthread_t Th1, Th2;

	pthread_create(&Th1, NULL, inc, NULL);
	pthread_create(&Th2, NULL, inc, NULL);

	pthread_join(Th1, NULL);
	pthread_join(Th2, NULL);

	printf("count = %d\n", count);
	if(count != 20000000) printf("SYNC Issued\n");

	return 0;
}

