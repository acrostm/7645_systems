/*
 * pipes.c
 * Communication between parent and children using a pipe
 * Parent will create the pipe and write some string to it
 * Chils will read from the pipe until there is nothing left to read and print the string
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
    char message[100];
    char valueRead;
    int childProcessId, childStatus;
    int index;
    int numRead, numWritten;

    strcpy(message, "Linux Programming");

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
            numRead = read(pipeDescriptor[0], &valueRead, sizeof(char));

            while (numRead > 0)
            {
                if (numRead == '\0')
                {
                    printf("\n");
                    break;
                }
                printf("Child read %c from pipe\n", valueRead);
                numRead = read(pipeDescriptor[0], &valueRead, sizeof(char));
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
            int size = strlen(message) + 1;
            numWritten = write(pipeDescriptor[1], message, size);
            if (numWritten < size)
            {
                printf("Partial write to pipe!\n");
                exit(EXIT_FAILURE);
            }


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
