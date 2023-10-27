#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

volatile int globalVariable = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct
{
    int numToDo;
    int numTimes;
} ThreadInput;

void *producer(void *input)
{
    int status;
    ThreadInput *input1 = (ThreadInput *)input;


    status = pthread_mutex_lock(&mutex);
    if (status != 0)
    {
        printf("Unable to lock mutex!\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < input1->numTimes; i++)
    {
        for (int j = 0; j < input1->numToDo; j++)
        {
            globalVariable++;
        }
    }

    /* unlock the mutex */
    status = pthread_mutex_unlock(&mutex);
    if (status != 0)
    {
        printf("Unable to unlock mutex!\n");
        exit(EXIT_FAILURE);
    }

    return NULL;
}

void *consumer(void *input)
{
    int status;
    ThreadInput input2 = *(ThreadInput *)input;
    /* lock the mutex */
    status = pthread_mutex_lock(&mutex);
    if (status != 0)
    {
        printf("Unable to lock mutex!\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < input2.numTimes; i++)
    {
        for (int j = 0; j < input2.numToDo; j++)
        {
            globalVariable--;
        }
    }

    /* unlock the mutex */
    status = pthread_mutex_unlock(&mutex);
    if (status != 0)
    {
        printf("Unable to unlock mutex!\n");
        exit(EXIT_FAILURE);
    }

    return NULL;
}

int main(int argc, char **argv)
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

    ThreadInput input1, input2;
    input1.numToDo = n1;
    input1.numTimes = s1;
    input2.numToDo = n2;
    input2.numTimes = s2;

    pthread_t thread1, thread2;
    int status;
    void *result;

    /* Create the threads */
    status = pthread_create(&thread1, NULL, producer, (void *)&input1);
    if (status != 0)
    {
        printf("Unable to create producer thread!\n");
        exit(EXIT_FAILURE);
    }

    status = pthread_create(&thread2, NULL, consumer, (void *)&input2);
    if (status != 0)
    {
        printf("Unable to create consumer thread!\n");
        exit(EXIT_FAILURE);
    }

    status = pthread_join(thread1, &result);
    if (status != 0)
    {
        printf("Unable to join producer thread!\n");
        exit(EXIT_FAILURE);
    }

    status = pthread_join(thread2, &result);
    if (status != 0)
    {
        printf("Unable to join consumer thread!\n");
        exit(EXIT_FAILURE);
    }

    printf("Final value of globalVariable = %d\n", globalVariable);
}
