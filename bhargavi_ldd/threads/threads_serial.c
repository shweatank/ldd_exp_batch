#include <stdio.h>
#include <pthread.h>
#include <unistd.h>   // for sleep()

#define MAX_THREAD 5

void *thread_func(void *id)
{
    long tid = (long)id;

    printf("Thread %ld started\n", tid);
    sleep(2);   // simulate work
    printf("Thread %ld finished\n", tid);

    return NULL;
}

int main()
{
    pthread_t thread[MAX_THREAD];
    int ret;

    for (int i = 0; i < MAX_THREAD; i++)
    {
        ret = pthread_create(&thread[i], NULL, thread_func, (void *)i);
        if (ret)
        {
            printf("Failed to create thread %d\n", i);
        }

        int ret1 = pthread_join(thread[i], NULL);
        if (ret1)
        {
            printf("Failed to join thread %d\n", i);
        }
        else
        {
            printf("Successfully joined thread %d\n", i);
        }
    }

    pthread_exit(NULL);
}

