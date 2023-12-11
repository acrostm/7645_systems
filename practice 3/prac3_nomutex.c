#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct {
    int id;
    int increment;
} Arg;

static int glob = 0;

static void* threadFunc(void* arg)
{
    int status;

    Arg *loc = (Arg *) arg;

    for(int i = 0; i < loc->increment; i++)
    {
        glob++;

        printf("THREAD ID: %d, new value of glob: %d\n", loc->id, glob);

        for(int j = 0; j < 1000000; j++)
        {
            /* Do nothing */
        }

    }

    return NULL;
}

int main(int argc, char** argv)
{
    pthread_t t1, t2;
    int status;
    // loops = (argc > 1) ? getInt(argv[1], GN_GT_0, "num-loops") : 10000000;
    Arg *loops1;
    loops1 = malloc(sizeof(Arg));
    loops1->id = 1;
    loops1->increment = 1000000;
    Arg *loops2;
    loops2 = malloc(sizeof(Arg));
    loops2->id = 2;
    loops2->increment = 1000000;
    status = pthread_create(&t1, NULL, threadFunc, loops1);
    if (status != 0)
    {
        printf("Unable to lock mutex!\n");
        exit(EXIT_FAILURE);
    }
    status = pthread_create(&t2, NULL, threadFunc, loops2);
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

    exit(EXIT_SUCCESS);
}
