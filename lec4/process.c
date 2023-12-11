/* print the process ID and the ID of the parent process */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>


int main(int argc, char** argv)
{
    printf("My process ID is %d\n", getpid());
    printf("My parent process ID is %d\n", getppid());

    exit(EXIT_SUCCESS);
}