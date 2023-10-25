#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

volatile int globalVariable = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

#define TRUE 1
#define FALSE 0

void* producer(void* input)
{
    int status;
    int numToDo = *(int*)input;
    /* lock the mutex */
    status = pthread_mutex_lock(&mutex);
    if(status != 0)
    {
        printf("Unable to lock mutex!\n");
        exit(EXIT_FAILURE);
    }

    /* consumer an item if there is a space available */
    globalVariable += numToDo;

    /* unlock the mutex */
    status = pthread_mutex_unlock(&mutex);
    if (status != 0)
    {
        printf("Unable to unlock mutex!\n");
        exit(EXIT_FAILURE);
    }

    return NULL;
}

void* consumer(void* input)
{
    int status;
    int numToDo = *(int*)input;
    /* lock the mutex */
    status = pthread_mutex_lock(&mutex);
    if(status != 0)
    {
        printf("Unable to lock mutex!\n");
        exit(EXIT_FAILURE);
    }

    /* consumer an item if there is a space available */
    globalVariable -= numToDo;

    /* unlock the mutex */
    status = pthread_mutex_unlock(&mutex);
    if (status != 0)
    {
        printf("Unable to unlock mutex!\n");
        exit(EXIT_FAILURE);
    }

    return NULL;

}

int main(int argc, char** argv)
{
    if (argc != 5 || strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0)
    {
        printf("Usage: ./threads <n1> <s1> <n2> <s2>\n");
        exit(EXIT_FAILURE);
    }

    int n1 = atoi(argv[1]);
    int s1 = atoi(argv[2]);
    int n2 = atoi(argv[3]);
    int s2 = atoi(argv[4]);

    pthread_t thread1, thread2;
    int status;
    int value1 = n1 * s1;
    int value2 = n2 * s2;
    void *result;

    /* Create the threads */
    status = pthread_create(&thread1, NULL, producer, (void*)&value1);
    if(status != 0)
    {
        printf("Unable to create producer thread!\n");
        exit(EXIT_FAILURE);
    }
    
    status = pthread_create(&thread2, NULL, consumer, (void*)&value2);
    if(status != 0)
    {
        printf("Unable to create consumer thread!\n");
        exit(EXIT_FAILURE);
    }

    status = pthread_join(thread1, &result);
    if(status != 0)
    {
        printf("Unable to join producer thread!\n");
        exit(EXIT_FAILURE);
    }


    status = pthread_join(thread2, &result);
    if(status != 0)
    {
        printf("Unable to join consumer thread!\n");
        exit(EXIT_FAILURE);
    }

    printf("Final value of globalVariable = %d\n", globalVariable);
}