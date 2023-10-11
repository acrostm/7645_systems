/* Lecture 5
 * topic:
 * 1. multithreaded programming
 * steps:
 * 1. parentthread will create two child threads
 * 2. first child will keep printing "Thread 1" 1000000 times
 * 3. second child will keep printing "Thread 2" 1000000 times
 * 4.
 * Compile as follows:
 * cc threads.c -o threads -pthread
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

volatile int globalVariable = 0;

/* Write the function that the threads must execute */
void* threadFunction(void* input)
{
    int value, index;
    value = *(int*)input;

    for(index = 0; index < 1000000; index++)
    {
        // printf("Thread %d\n", value);
        globalVariable++;
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
