/*
    compile as :
     cc cat.c -o cat

     Run as:
     ./cat <file1.txt> <file2.txt> <file3.txt> ... <file n.txt>

     OR
     ./cat --help / ./cat -h / ./cat -help
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define CHUNK_SIZE 1024

int main (int argc, char ** argv)
{
    int numFiles = argc - 1;
    int file;
    int numRead, numWritten;
    char buffer[CHUNK_SIZE];
    int closeStatus;

    if (argc < 2 || strcmp(argv[1], "-help") == 0 || strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)
    {
        printf("Usage: %s <file1.txt> <file2.txt> <file3.txt> ... <file n.txt>"
               "\tthe content of these files will be added up and show on terminal."
               "\nOptions: \n\t --help/-h \tshow help\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    for (int i = 1; i < numFiles + 1; i++)
    {
        file = open(argv[i], O_RDONLY, S_IRUSR | S_IRGRP | S_IROTH);
        if (file == -1)
        {
            printf("%s cannot be read", argv[i]);
            exit(EXIT_FAILURE);
        }

        numRead = read(file, buffer, CHUNK_SIZE);
        if (numRead == -1)
        {
            printf("Unable to read %s\n", argv[i]);
            exit(EXIT_FAILURE);
        }

        while (numRead > 0)
        {
            numWritten = write(1, buffer, numRead);

            if (numRead != numWritten)
            {
                printf("Failed writting %s", argv[i]);
            }

            numRead = read(file, buffer, CHUNK_SIZE);
            if (numRead == -1)
            {
                printf("Unable to read %s\n", argv[i]);
                exit(EXIT_FAILURE);
            }
        }

        closeStatus = close(file);

        if (closeStatus == -1)
        {
            printf("Failed closing %s", argv[i]);
        }

    }
    exit(EXIT_SUCCESS);
}