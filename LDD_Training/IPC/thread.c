#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
int count = 0;
void *print_msg(void *ptr) {
	count++;
	char *msg = (char *)ptr;
	printf("%s - %d\n",msg, count);
}

void *print_msg1(void *ptr) {
	count++;
	char *msg = (char *)ptr;
	printf("%s - %d\n",msg, count);
}
int main() {
	
	pthread_t th1, th2, th3;
	int ret1, ret2, ret3;

	//Create thread
	ret1 = pthread_create( &th1, NULL, print_msg, (void *)"I am thread 1\n");
	ret2 = pthread_create( &th2, NULL, print_msg1, (void *)"I am thread 2\n");
	ret3 = pthread_create( &th3, NULL, print_msg, (void *)"I am thread 3\n");

	sleep(10);
	pthread_join(th1, NULL);
	pthread_join(th2, NULL);
	pthread_join(th3, NULL);

	exit(0);

}
