/* 
 * Example of synchronization using a pipe.
 * Author: Sourav Mukherjee (sourav@fdu.edu)
 * 
 * Imagine a parent process P creates two child processes C1 and C2.
 * Each child process must complete two tasks T1 and T2.
 * After C1 and C2 have both completed T1, the parent must print:
 * "All children have completed their first task."
 * After C1 and C2 have completed T2 and called exit, the parent must print:
 * "All children have completed their second task as well."
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> /* for p_id */
#include <unistd.h> /* for pipe, sleep, fork  */
#include <sys/wait.h> /* for wait */

# define BUF_SIZE 1

void doTask(int childIndex, char* taskName)
{
    printf("Child %d completed task: %s\n", childIndex, taskName);
}

int main(int argc, char *argv[])
{
    int pd[2]; /* pipe descriptors */
    pid_t child;
    int status, childStatus, index;
    char buffer[BUF_SIZE];
    int numRead;

    status = pipe(pd);
    if (status == -1){
        printf("Could not open pipe.\n");
        exit(EXIT_FAILURE);
    }

    for(index = 0; index < 2; index++){
        child = fork();
        switch(child){
            case -1:
                printf("Could not created child.\n");
                exit(EXIT_FAILURE);

            case 0: /* child process */
                status = close(pd[0]);
                if(status == -1){
                    printf("Child process could not close read end of pipe.\n");
                    _exit(EXIT_FAILURE);
                }

                
                doTask(index, "T1");

                status = close(pd[1]);
                if(status == -1){
                    printf("Child process could not close write end of pipe.\n");
                    _exit(EXIT_FAILURE);
                }

                sleep(2);

                doTask(index, "T2");

                _exit(EXIT_SUCCESS);

            default:
                break;
        }
    }

    /* Parent process */
    status = close(pd[1]);
    if (status == -1){
            printf("Parent process could not close write end of pipe.\n");
            exit(EXIT_FAILURE);
    }  

    numRead = read(pd[0], buffer, BUF_SIZE);
    if(numRead != 0){
        printf("Parent process did not receive EOF as expected.\n");
        exit(EXIT_FAILURE);
    }
    
    printf("All children have completed their first task.\n");

    status = close(pd[0]);
    if (status == -1){
        printf("Parent process could not close read end of pipe.\n");
        exit(EXIT_FAILURE);
    }

    for (index = 0; index < 2; index++){
        status = wait(&childStatus);
        if (status == -1)
        {
            printf("Parent process failed to wait for child process.\n");
            exit(EXIT_FAILURE);
        }
    }

    printf("All children have completed their second task as well.\n");
    exit(EXIT_SUCCESS);
}