/*
 * pipes.c
 * Communication between parent and children using a pipe
 * Parent will create the pipe and write some integers to it
 * Chils will read from the pipe until there is nothing left to read
 * How to:
 * 1. read and wirte a struct instead of an int
 * 2. child read and parent write, child write and parent read, reversed process of this program
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char** argv)
{
    int pipeDescriptor[2];
    int status;
    int valuesToWrite[] = {1,3,5,7,9};
    int valueRead;
    int childProcessId, childStatus;
    int index;
    int numRead, numWritten;

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

        case 0: /* child process */
            /* Close the write descriptor of the pipe */
            status = close(pipeDescriptor[1]);
            if (status == -1)
            {
                printf("Child unable to close write descriptor!\n");
                _exit(EXIT_FAILURE);
            }

            /* Read from the pipe until there is nothing to read */
            numRead = read(pipeDescriptor[0], &valueRead, sizeof(int));

            while (numRead > 0)
            {
                printf("Child read %d from pipe\n", valueRead);
                numRead = read(pipeDescriptor[0], &valueRead, sizeof(int));
            }

            if (numRead == -1)
            {
                printf("Child unable to read from pipe!\n");
                _exit(EXIT_FAILURE);
            }

            /* Close the read descriptor of the pipe */
            status = close(pipeDescriptor[0]);
            if (status == -1)
            {
                printf("Child unable to close read descriptor!\n");
                _exit(EXIT_FAILURE);
            }
            
            _exit(EXIT_SUCCESS);
        default: /* Parent process*/
            /* Close the read descriptor of the pipe */
            status = close(pipeDescriptor[0]);
            if (status == -1)
            {
                printf("Parent unable to close read descriptor!\n");
                exit(EXIT_FAILURE);
            }

            /* Write the values from the array to the pipe */
            int size = 5*sizeof(int);
            numWritten = write(pipeDescriptor[1], valuesToWrite, size);
            if (numWritten < size)
            {
                printf("Partial write to pipe!\n");
                exit(EXIT_FAILURE);
            }

            // for (index = 0; index < 5; index++)
            // {
            //     numWritten = write(pipeDescriptor[1], &valuesToWrite[index], sizeof(int));
            //     if (numWritten < sizeof(int))
            //     {
            //         printf("Partial write to pipe!\n");
            //         exit(EXIT_FAILURE);
            //     }
            // }

            /* Close the write descriptor of the pipe */
            status = close(pipeDescriptor[1]);
            if (status == -1)
            {
                printf("Parent unable to close write descriptor!\n");
                exit(EXIT_FAILURE);
            }

            childProcessId = wait(&childStatus);
            if (childStatus == -1)
            {
                printf("Unable to wait for child process!\n");
                exit(EXIT_FAILURE);
            }

            exit(EXIT_SUCCESS);
    }
}
