#include <unistd.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char **argv)
{
    pid_t childProcessX;
    pid_t childProcessY;
    pid_t childProcessZ ;
    int status;

    childProcessX = fork();
    switch (childProcessX)
    {
        case -1:
            printf("Unable to create a child process Z!");
            exit(EXIT_FAILURE);

        case 0:
            printf("Parent process p %d created child process x %d\n", getppid(), getpid());

            printf("Child process x going to sleep for 10 seconds\n");
            sleep(10);
            printf("Exiting child process x %d\n", getpid());
            exit(EXIT_SUCCESS);
    }

    childProcessY = fork();
    switch (childProcessY)
    {
        case -1:
            printf("Unable to create a child process Y!");
            exit(EXIT_FAILURE);

        case 0:
            printf("Parent process p %d created child process y %d\n", getppid(), getpid());

            printf("Child process y going to sleep for 5 seconds\n");
            sleep(5);
            printf("Exiting child process y %d\n", getpid());
            exit(EXIT_SUCCESS);
    }

    childProcessZ = fork();
    switch (childProcessZ)
    {
        case -1:
            printf("Unable to create a child process Z!");
            exit(EXIT_FAILURE);

        case 0:
            printf("Parent process p %d created child process z %d\n", getppid(), getpid());

            printf("Child process z going to sleep for 2 seconds\n");
            sleep(2);
            printf("Exiting child process z %d\n", getpid());
            exit(EXIT_SUCCESS);
    }

    /* scenario 1 */

    /* scenario 2 */
    // wait(&status);
    // wait(&status);
    // wait(&status);


    /* scenario 3 */
    waitpid(childProcessX, &status, 0);
    waitpid(childProcessY, &status, 0);
    waitpid(childProcessZ, &status, 0);

}
