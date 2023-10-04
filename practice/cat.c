/* cat.c */

/*
 * Compile as :
 * cc cat.c -o cat
 *
 * Run as:
 * ./cat <file1.txt> <file2.txt> <file3.txt> ... <fileN.txt>
 *
 * OR
 * ./cat --help
 * ./cat -h
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#defin CHUNK_SIZE 1024

int main (int argc, char **argv)
{
    int sourceFd;
    char buffer[CHUNK_SIZE];
    int status;

    if (argc < 2 || strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)
    {
        printf("Usage: %s <file1.txt> <file2.txt> <file3.txt> ... <fileN.txt>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
}