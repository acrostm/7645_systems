/*
 * Lecture 6
 * topics:
 * 1. continue multithreaded programming
 * 2. interprocess communication
 */

// mutex solution code for race condition

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

volatile int globalVariable = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


/* Write the function that the threads must execute */
void* threadFunction(void* input)
{    int value, index, status;
    value = *(int*)input;

    for(index = 0; index < 1000000; index++)
    {
        /* Lock the mutex */
        status = pthread_mutex_lock(&mutex);
        if (status != 0)
        {
            printf("Unable to lock mutex!\n");
            exit(EXIT_FAILURE);
        }
        globalVariable++;
        /* Unlock the mutex */
        status = pthread_mutex_unlock(&mutex);
        if (status != 0)
        {
            printf("Unable to unlock mutex!\n");
            exit(EXIT_FAILURE);
        }
    }

    return NULL;
}

int main(int argc, char** argv)
{
    pthread_t thread1, thread2;
    int status;
    int value1 = 1, value2 = 2;
    void* result;

    /* Create the threads */
    status = pthread_create(&thread1, NULL, threadFunction, (void*)&value1);
    if(status != 0)
    {
        printf("Unable to create thread 1!\n");
        exit(EXIT_FAILURE);
    }
    
    status = pthread_create(&thread2, NULL, threadFunction, (void*)&value2);
    if(status != 0)
    {
        printf("Unable to create thread 2!\n");
        exit(EXIT_FAILURE);
    }

    status = pthread_join(thread1, &result);
    if(status != 0)
    {
        printf("Unable to join thread 1!\n");
        exit(EXIT_FAILURE);
    }


    status = pthread_join(thread2, &result);
    if(status != 0)
    {
        printf("Unable to join thread 2!\n");
        exit(EXIT_FAILURE);
    }

    printf("Global variable: %d\n", globalVariable);
    
}

