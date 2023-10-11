#include <unistd.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char** argv)
{
    if (argc < 2 || strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0)
    {
        printf("Usage: %s <n_1> <n_2> ...<n_k>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    pid_t procId[argc - 1];

    for (int i = 0; i < argc - 1; i++)
    {
        procId[i] = fork();
        switch (procId[i])
        {
            case -1:
                printf("Unable to create a child process %d!\n", i+1);
                exit(EXIT_FAILURE);

            case 0:
                printf("Creating child process %d.\n", i+1);

                sleep(atoi(argv[i+1]));
                _exit(EXIT_SUCCESS);
        }

    }

    for(int k = argc - 2; k >= 0 ; k--)
    {
        int status;
        waitpid(procId[k], &status, 0);
        printf("Child process %d completed.\n", k+1);
    }

    exit(EXIT_SUCCESS);
}