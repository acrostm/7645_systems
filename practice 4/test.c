/* Pipe test
 * compiles as cc test.c -o pipe
 * run as ./pipe <n1> <n2> <n3> <n4> <n5> ...
 * the parent process should read and write one or more numbers to the pipe
 * and the child process read the numbers and prints their sums
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>


int main(int argc, char** argv)
{
    int pipeDescriptor[2];
    int status;
    int childProcessId, childStatus;
    int numRead, numWritten;
    int sum = 0;
    int numSize = argc - 1;
    int* numbers = malloc(sizeof(int) * numSize);

    if(argc < 2 || strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0)
    {
        printf("Usage: ./pipe <n1> <n2> <n3> <n4> <n5> ...\n");
        exit(EXIT_FAILURE);
    }

    /* Convert command line arguments to integers */
    for (int i = 0; i < numSize; i++)
    {
        numbers[i] = atoi(argv[i + 1]);
    }

    /* Parent must create pipe before creating the child process */
    status = pipe(pipeDescriptor);
    if (status == -1)
    {
        printf("Unable to create pipe!\n");
        exit(EXIT_FAILURE);
    }

    /* Create the child process */
    childProcessId = fork();
    switch(childProcessId)
    {
        case -1:
            printf("Unable to fork!\n");
            exit(EXIT_FAILURE);
        case 0:
            status = close(pipeDescriptor[1]);
            if (status == -1)
            {
                printf("Child unable to close write descriptor!\n");
                _exit(EXIT_FAILURE);
            }

            numRead = read(pipeDescriptor[0], numbers, sizeof(int) * numSize);
            while (numRead > 0)
            {
                for (int i = 0; i < numSize; i++)
                {
                    sum += numbers[i];
                }
                printf("%d\n", sum);
                numRead = read(pipeDescriptor[0], numbers, sizeof(int) * numSize);
            }

            if (numRead == -1)
            {
                printf("Child unable to read from pipe!\n");
                _exit(EXIT_FAILURE);
            }

            status = close(pipeDescriptor[0]);
            if (status == -1)
            {
                printf("Child unable to close read descriptor!\n");
                _exit(EXIT_FAILURE);
            }

            _exit(EXIT_SUCCESS);

        default:
            status = close(pipeDescriptor[0]);
            if (status == -1)
            {
                printf("Parent unable to close read descriptor!\n");
                exit(EXIT_FAILURE);
            }

            for(int i = 0; i < numSize; i++)
            {
                numWritten = write(pipeDescriptor[1], &numbers[i], sizeof(int));
                if (numWritten < sizeof(int))
                {
                    printf("Parent unable to write to pipe!\n");
                    exit(EXIT_FAILURE);
                }
            }

            status = close(pipeDescriptor[1]);
            if (status == -1)
            {
                printf("Parent unable to close write descriptor!\n");
                exit(EXIT_FAILURE);
            }

            wait(&childStatus);
            if (childStatus == -1)
            {
                printf("Unable to wait for child process!\n");
                exit(EXIT_FAILURE);
            }
            exit(EXIT_SUCCESS);
    }
}