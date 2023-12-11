#include <pthread.h>
#include "tlpi_hdr.h"
static int glob = 0;
static void *
threadFunc(void *arg)
{
    int loops = *((int *)arg);
    int loc, j;
    /* Loop 'arg' times incrementing 'glob' */
    for (j = 0; j < loops; j++)
    {
        loc = glob;
        loc++;
        glob = loc;
    }
    return NULL;
}


int main(int argc, char *argv[])
{
    pthread_t t1, t2;
    int loops, status;
    loops = (argc > 1) ? getInt(argv[1], GN_GT_0, "num-loops") : 10000000;
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
