#include <pthread.h>    /* Threading head file */
#include <sys/types.h>  /* Type definitions used by many programs */
#include <stdio.h>      /* Standard I/O functions */
#include <stdlib.h>     /* Prototypes of commonly used library functions,
                           plus EXIT_SUCCESS and EXIT_FAILURE constants */
#include <unistd.h>     /* Prototypes for many system calls */
#include <errno.h>      /* Declares errno and defines error constants */
#include <string.h>     /* Commonly used string-handling functions */
#include <stdbool.h>    /* 'bool' type plus 'true' and 'false' constants */
// #include "tlpi_hdr.h"

static int glob = 0;
static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

static void *threadFunc(void *arg)
{
    int increment = *((int *) arg);
    int status, local;

    for(int i = 0; i < increment; i++)
    {
        status = pthread_mutex_lock(&mtx);
        if(status != 0)
        {
            printf("Unable to lock mutex!\n");
            exit(EXIT_FAILURE);
        }

        local = glob;
        local++;
        glob = local;

        status = pthread_mutex_unlock(&mtx);
        if(status != 0)
        {
            printf("Unable to unlock mutex!\n");
            exit(EXIT_FAILURE);
        }
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t t1, t2;
    int loops, status;
    // loops = (argc > 1) ? getInt(argv[1], GN_GT_0, "num-loops") : 10000000;
    loops = 1000000;
    status = pthread_create(&t1, NULL, threadFunc, &loops);
    if (status != 0)
    {
        printf("Unable to lock mutex!\n");
        exit(EXIT_FAILURE);
    }
    status = pthread_create(&t2, NULL, threadFunc, &loops);
    if (status != 0)
    {
        printf("Unable to lock mutex!\n");
        exit(EXIT_FAILURE);
    }
    status = pthread_join(t1, NULL);
    if (status != 0)
    {
        printf("Unable to lock mutex!\n");
        exit(EXIT_FAILURE);
    }
    status = pthread_join(t2, NULL);
    if (status != 0)
    {
        printf("Unable to lock mutex!\n");
        exit(EXIT_FAILURE);
    }
    printf("glob = %d\n", glob);
    exit(EXIT_SUCCESS);
}
