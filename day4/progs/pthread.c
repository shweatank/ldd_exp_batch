#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *mssage_func(void *ptr);

int main(){
	pthread_t p1,p2,p3;

	char *msg1 = "message1";
	char *msg2 = "message2";
	char *msg3 = "message3";
	
	int ret1, ret2, ret3;

	ret1 = pthread_create(&p1,NULL,mssage_func,(void *)msg1);
	ret2 = pthread_create(&p2,NULL,mssage_func,(void *)msg2);
	ret3 = pthread_create(&p3,NULL,mssage_func,(void *)msg3);

	pthread_join(p1, NULL);
	pthread_join(p2, NULL);
	pthread_join(p3, NULL);

	printf("p1 returns : %d\n",ret1);
	printf("p2 returns : %d\n",ret2);
	printf("p3 returns : %d\n",ret3);
	exit(0);
}

void *mssage_func(void *ptr){

	char *msg;
	msg = (char*)ptr;
	printf("%s\n",msg);
}
