/* parent will create a child process
 * and wait for the child to complete
 * Child will sleep for 5 seconds and exit */

#include <unistd.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

void saysBye()
{
    printf("process %d says ByeBye!\n", getpid());
}

int main(int argc, char** argv)
{
    pid_t childProcessId;
    int status;

    childProcessId = fork();

    atexit(saysBye);

    switch(childProcessId)
    {
        case -1:
            
            printf("failed to create the child process\n");
            exit(EXIT_FAILURE);

        case 0:
            
            printf("I am the child process with id %d\n", getpid());
            printf("My parent's process ID is %d\n", getppid());

            char* const argument[] = {"cc", "--version", NULL};

            execv("/usr/bin/cc", argument);

            /* the process should not go under this line
             * show the error failed under this
             */

            printf("the execv() has failed, the child process is going to sleep for 5s\n");
            sleep(5);

            _exit(EXIT_SUCCESS);

        default:

            printf("Created a child with a id %d\n", childProcessId);
            wait(&status);
            printf("child process completed with status %d\n", status);
            exit(EXIT_SUCCESS);

    }
}