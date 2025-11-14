#include<stdio.h>
#include<unistd.h>
#include<pthread.h>


void *fun(void *arg)
{
     printf("%s",(char *)arg);
}   
int main()
{
    pthread_t  p1,p2,p3;

    pthread_create(&p1,NULL,fun,"sai");
    pthread_create(&p2,NULL,fun,"varshath");
    pthread_create(&p3,NULL,fun,"konduri");
    
    pthread_join(p1,NULL);
    pthread_join(p2,NULL);
    pthread_join(p3,NULL);
    
    return 0;
}
