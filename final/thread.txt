
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

static volatile int globalVariable = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

static void *increment(void *arg)
{
    int loops = *((int *)arg);
    int status;
    int j;

    for (j = 0; j < loops; j++)
    {
        status = pthread_mutex_lock(&mutex);
        if (status == -1)
        {
            printf("Failed to lock mutex\n");
            exit(EXIT_FAILURE);
        }
        globalVariable++;
        status = pthread_mutex_unlock(&mutex);
        if (status == -1)
        {
            printf("Failed to unlock mutex\n");
            exit(EXIT_FAILURE);
        }
    }

    return NULL;
}

static void *decrement(void *arg)
{
    int loops = *((int *)arg);
    int status;
    int j;

    for (j = 0; j < loops; j++)
    {
        status = pthread_mutex_lock(&mutex);
        if (status == -1)
        {
            printf("Failed to lock mutex\n");
            exit(EXIT_FAILURE);
        }
        globalVariable--;
        status = pthread_mutex_unlock(&mutex);
        if (status == -1)
        {
            printf("Failed to unlock mutex\n");
            exit(EXIT_FAILURE);
        }
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t t1, t2;
    int loops, s;

    loops = 10000000;

    s = pthread_create(&t1, NULL, increment, &loops);
    if (s != 0)
    {
        printf("Failed to create thread\n");
        exit(1);
    }

    s = pthread_create(&t2, NULL, decrement, &loops);
    if (s != 0)
    {
        printf("Failed to create thread\n");
        exit(1);
    }

    s = pthread_join(t1, NULL);
    if (s != 0)
    {
        printf("Failed to join thread\n");
        exit(1);
    }

    s = pthread_join(t2, NULL);
    if (s != 0)
    {
        printf("Failed to join thread\n");
        exit(1);
    }

    printf("%d\n", globalVariable);
    exit(EXIT_SUCCESS);
}
