/* Practice set 4 Pipe and FIFO */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char** argv)
{
    char message[100];
    int pipeDescriptor[2];
    int status;
    pid_t childProcessId, childStatus;
    int numRead, numWritten;

    strcpy(message, argv[1]);
    /* Parent create the pipe */
    status = pipe(pipeDescriptor);
    if (status == -1)
    {
        printf("Unable to create pipe!\n");
        exit(EXIT_FAILURE);
    }
    
    /* Create child */
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

            numRead = read(pipeDescriptor[0], message, 100);
            while(numRead > 0)
            {
                printf("Child read \"%s\" from pipe\nSize of the message read is %d\n", message, (unsigned int)strlen(message));
                numRead = read(pipeDescriptor[0], message, 100);
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
            if(status == -1)
            {
                printf("Parent unable to close read descriptor!\n");
                exit(EXIT_FAILURE);
            }

            int size = strlen(message) + 1;
            numWritten = write(pipeDescriptor[1], message, size);
            if(numWritten < size)
            {
                printf("Parent unable to write to pipe!\n");
                exit(EXIT_FAILURE);
            }

            status = close(pipeDescriptor[1]);
            if(status == -1)
            {
                printf("Parent unable to close write descriptor!\n");
                exit(EXIT_FAILURE);
            }

            wait(&childStatus);
            if(childStatus == -1)
            {
                printf("Unable to wait for child process!\n");
                exit(EXIT_FAILURE);
            }

            exit(EXIT_SUCCESS);
    }
}