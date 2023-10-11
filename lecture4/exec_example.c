/* Example Program for CSCI-7645
 * Author: Sourav Mukherjee, sourav@fdu.edu
 * This program is meant to illustrate fork, system, execv
 * Imagine that the parent process calls fork.
 * The child process executes the command "cc --version" using execv,
 * while the parent process executes the command "ls" using system.
 */

#include <stdio.h> /* for printf */
#include <stdlib.h> /* for exit and system */
#include <sys/types.h> /* for getpid, fork, and execv */
#include <unistd.h> /* for getpid, fork, and execv */
#include <sys/wait.h> /* for wait */

int main(int argc, char** argv)
{
    pid_t processId = fork();
    int childStatus;

    switch(processId){
        case -1:
            printf("fork failed.\n");
            exit(EXIT_FAILURE);

        case 0: /* Child process*/
            printf(
                "Child process %d is about to run \"cc --version\"\n",
                getpid()); 
            char* const arguments[] = {"cc", "--version", NULL}; /* The last element of this string array must be NULL */
            execv("/usr/bin/cc", arguments);
            /* The code should never reach the line below;
             * if it does, then execv has failed.
             */
            printf("execv failed.\n");
            _exit(EXIT_FAILURE);

        default: /* Parent process*/
            wait(&childStatus);
            printf(
                "Parent process %d is about to run \"ls\"\n",
                getpid()
            );
            system("ls");
            exit(EXIT_SUCCESS);

    }    
}