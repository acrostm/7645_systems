/*
    Producer thread will keep incrementing forever numUnits by 1.
    Consumer thread will keep decrementing forever numUnits by 1.
    MAX_BUFFER_SIZE is the maximum size of the buffer.
    Constraints (all three must be satisfied simultaneously):
    (1) Mutual exclusion: producer and consumer cannot access numUnits at the same time
    (2) No overflow: numUnits <= MAX_BUFFER_SIZE
    (3) No underflow: numUnits >= 0
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_BUFFER_SIZE 5
#define TRUE 1
#define FALSE 0

volatile int numUnits = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t spaceAvailable = PTHREAD_COND_INITIALIZER;
pthread_cond_t unitAvailable = PTHREAD_COND_INITIALIZER;

void* producer(void* input)
{
    int status; 

    while(TRUE)
    {

        /* lock the mutex */
        status = pthread_mutex_lock(&mutex);
        if(status != 0)
        {
            printf("Unable to lock mutex!\n");
            exit(EXIT_FAILURE);
        }

        /* produce an item if there is a space available */
        while(!(numUnits < MAX_BUFFER_SIZE))
        {
            printf("Producer must block\n");
            /* Unlocks the mutex before putting the producer into blocked state */
            status = pthread_cond_wait(&spaceAvailable, &mutex); 
            if (status != 0)
            {
                printf("Unable to wait on spaceAvailable!\n");
                exit(EXIT_FAILURE);
            }
        }

        numUnits++;
        printf("Produced an item. Total number of items: %d\n", numUnits);

        if (numUnits == 1)
        {
            status = pthread_cond_signal(&unitAvailable);
            if (status != 0)
            {
                printf("Unable to signal unitAvailable!\n");
                exit(EXIT_FAILURE);
            }
            
        }

        /* unlock the mutex */
        status = pthread_mutex_unlock(&mutex);
        if (status != 0)
        {
            printf("Unable to unlock mutex!\n");
            exit(EXIT_FAILURE);
        }

        sleep(1);

    }


    return NULL;
}


void* consumer(void* input)
{
    int status; 

    while(TRUE)
    {

        /* lock the mutex */
        status = pthread_mutex_lock(&mutex);
        if(status != 0)
        {
            printf("Unable to lock mutex!\n");
            exit(EXIT_FAILURE);
        }


        while (!(numUnits > 0))
        {
            printf("Consumer must block\n");
            /* Unlocks the mutex before putting the consumer into blocked state */
            status = pthread_cond_wait(&unitAvailable, &mutex);
            if (status != 0)
            {
                printf("Unable to wait on unitAvailable!\n");
                exit(EXIT_FAILURE);
            }
        }

        numUnits--;
        printf("Consumed an item. Total number of items: %d\n", numUnits);

        if (numUnits == MAX_BUFFER_SIZE - 1) /* Consumer must wake up producer */
        {
            status = pthread_cond_signal(&spaceAvailable);
            if (status != 0)
            {
                printf("Unable to signal spaceAvailable!\n");
                exit(EXIT_FAILURE);
            }
        }

        /* unlock the mutex */
        status = pthread_mutex_unlock(&mutex);
        if (status != 0)
        {
            printf("Unable to unlock mutex!\n");
            exit(EXIT_FAILURE);
        }
        sleep(2);

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

    printf("numUnits: %d\n", numUnits);
    
}
