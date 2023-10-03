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
    int dir_fd;
    char *directory_path = argv[1];
    DIR *dir;

    if (argc != 2 || strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0)
    {
        printf("Usage: %s <directory>\nOptions: \n\t --help/-h \tshow help\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    dir_fd = open(directory_path, O_RDONLY | O_DIRECTORY);
    if (dir_fd == -1)
    {
        printf("Unable to open directory %s\n", directory_path);
        exit(EXIT_FAILURE);
    }

    dir = readdir(dir_fd);
    while (dir != NULL)
        {
            printf("%s\n", dir->d_name);
            dir = readdir(dir_fd);
        }

    closedir(dir_fd);

    exit(EXIT_SUCCESS);

}