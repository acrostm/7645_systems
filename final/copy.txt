#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define CHUNK_SIZE 1024

int main(int argc, char **argv)
{
    int sourceFd, destinationFd;
    char buffer[CHUNK_SIZE];
    int status;

    if (argc != 3 || strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0)
    {
        fprintf(stderr, "%s <source> <destination>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    sourceFd = open(argv[1], O_RDONLY);
    if (sourceFd == -1)
    {
        perror("Failed to open source file");
        exit(EXIT_FAILURE);
    }

    if (access(argv[2], F_OK) != -1)
    {
        printf("Destination file already exists.\n");
        close(sourceFd);
        exit(EXIT_FAILURE);
    }

    destinationFd = open(
        argv[2],
        O_CREAT | O_EXCL | O_WRONLY | O_TRUNC,
        S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH
    );

    if (destinationFd == -1)
    {
        perror("Failed to create destination file");
        close(sourceFd);
        exit(EXIT_FAILURE);
    }

    int numRead, numWritten;
    numRead = read(sourceFd, buffer, CHUNK_SIZE);

    while (numRead > 0)
    {
        numWritten = write(destinationFd, buffer, numRead);

        if (numWritten < numRead)
        {
            perror("Partial write error");
            close(sourceFd);
            close(destinationFd);
            exit(EXIT_FAILURE);
        }

        numRead = read(sourceFd, buffer, CHUNK_SIZE);
    }

    status = close(sourceFd);

    if (status == -1)
    {
        perror("Failed to close source file");
        exit(EXIT_FAILURE);
    }

    status = close(destinationFd);
    if (status == -1)
    {
        perror("Failed to close destination file");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
