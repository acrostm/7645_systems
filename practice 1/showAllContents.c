#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>

#define CHUNK_SIZE 1024

int main(int argc, char **argv)
{
    int dir_fd;
    char *directory_path = argv[1];
    DIR *dir;
    int depth = 0;

    if (argc != 2 || strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0)
    {
        printf("Usage: %s <directory>\nOptions: \n\t --help/-h \tshow help\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    dir = opendir(directory_path);
    if (dir == NULL)
    {
        printf("Unable to open directory %s\n", directory_path);
        exit(EXIT_FAILURE);
    }

    struct dirent *entry;
    entry = readdir(dir);
    while (entry != NULL)
    {
        if (entry->d_name == "." || entry->d_name == "..")
        {
            continue;
        }   
        DIR* isDir = opendir(entry->d_name);

        if (isDir != NULL)
        {
            depth++;
        }

        for(int i = 0; i < depth; i++)
        {
            printf("\t");
        }

        printf("%s\n", entry->d_name);
        entry = readdir(dir);
    }

    closedir(dir);

    exit(EXIT_SUCCESS);
}
