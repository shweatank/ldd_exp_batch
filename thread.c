#include<stdio.h>
#include<pthread.h>

void *fun(char *x){
    printf("%s\n",x);
}
int main(){
    pthread_t thread1, thread2, thread3;
    char *ch1="thread1";
    char *ch2="thread2";
    char *ch3="thread3";
    pthread_create(&thread1, NULL, fun, (void*)ch1);
    pthread_create(&thread2, NULL, fun, (void*)ch2);
    pthread_create(&thread3, NULL, fun, (void*)ch3);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);
}