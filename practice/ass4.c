#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

typedef struct {
    char name[100];
    int count;
} NamePair;

int main(int argc, char** argv)
{
    int pipeDescriptor[2];
    int status;
    int childProcessId, childStatus;
    int numRead, numWritten;

    if(argc < 3 || argc % 2 != 1 || strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0)
    {
        printf("Usage: %s <name 1> <count 1> <name 2> <count 2> ...\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    status = pipe(pipeDescriptor);
    if(status == -1)
    {
        printf("Unable to create pipe!\n");
        exit(EXIT_FAILURE);
    }

    childProcessId = fork();
    switch(childProcessId)
    {
        case -1:
            printf("Unable to fork!\n");
            exit(EXIT_FAILURE);
        case 0:
            status = close(pipeDescriptor[1]);
            if(status == -1)
            {
                printf("Child unable to close write descriptor!\n");
                _exit(EXIT_FAILURE);
            }

            NamePair namePairChild;
            numRead = read(pipeDescriptor[0], &namePairChild, sizeof(NamePair));
            while (numRead > 0) 
            {
                printf("Name: %s, count: %d\n", namePairChild.name, namePairChild.count);
                numRead = read(pipeDescriptor[0], &namePairChild, sizeof(NamePair));
            }

            status = close(pipeDescriptor[0]);
            if(status == -1)
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

            for (int i = 1; i < argc; i += 2) 
            {
                NamePair namePair;
                strncpy(namePair.name, argv[i], sizeof(namePair.name) - 1);
                namePair.count = atoi(argv[i + 1]);
                numWritten = write(pipeDescriptor[1], &namePair, sizeof(NamePair));
                if(numWritten < sizeof(NamePair))
                {
                    printf("Parent unable to write to pipe!\n");
                    exit(EXIT_FAILURE);
                }
            }

            status = close(pipeDescriptor[1]);
            if(status == -1)
            {
                printf("Parent unable to close write descriptor!\n");
                exit(EXIT_FAILURE);
            }

            childProcessId = wait(&childStatus);
            if(childStatus == -1)
            {
                printf("Parent unable to wait for child!\n");
                exit(EXIT_FAILURE);
            }

            exit(EXIT_SUCCESS);
    }
}
