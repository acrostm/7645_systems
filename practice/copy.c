// copy.c

/*
    compile as :
     cc copy.c -o copy

     Run as:
     ./copy <source> <destination>

     OR
     ./copy -help
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define CHUNK_SIZE 1024

int main(int argc, char ** argv)
{   
    int sourceFd, destinationFd;
    char buffer[CHUNK_SIZE];
    int status;

    if ((argc != 3 && argc != 4) || strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0)
    {
        printf("Usage: %s <source> <destination>\nOptions: \n\t "
               "--help/-h \tshow help\n\t --append/-a \tcopy in append mode, "
               "the content will be appended to the file if it exits.\n", argv[0]);
        exit(EXIT_FAILURE);      
    }

    sourceFd = open(argv[1], O_RDONLY);
    if (sourceFd == -1)
    {
        printf("No such file on directory %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }
 
    if (argc == 4 && (strcmp(argv[3], "-a") == 0 || strcmp(argv[3], "--append") == 0))
    {
        destinationFd = open(
                argv[2],
                O_CREAT | O_WRONLY | O_APPEND,
                // TRUNC or APPEND, to replace the output file or to add to the end of the file
                // O_EXCL create fail when file exist and this floag only avalable if O_CREAT is there
                S_IRUSR | S_IWUSR |  S_IRGRP | S_IWGRP | S_IROTH
        );
    } else {
        destinationFd = open(
                argv[2],
                O_CREAT | O_EXCL | O_WRONLY | O_TRUNC,
                // TRUNC or APPEND, to replace the output file or to add to the end of the file
                // O_EXCL create fail when file exist and this floag only avalable if O_CREAT is there
                S_IRUSR | S_IWUSR |  S_IRGRP | S_IWGRP | S_IROTH
        );
    }

    if (destinationFd == -1)
    {
        printf("Fail to create/open file %s\n", argv[2]);
        exit(EXIT_FAILURE);
    }

    /* -rw-rw-r-- */
    /* permission -rwxrwxr-x |3 digi userpermission|3 digi grouppermission|3digi otherpermission|
     * r-read w-write x-execution */

    /* read from source file and write to destination */
    /* read the first chunk */
    int numRead, numWritten;
    // numRead == numWritten to avoid partial write without awareness, because memory not enough
    numRead = read(sourceFd, buffer, CHUNK_SIZE);

    while(numRead  > 0) 
    {
        /* Write this chunk to the destination */
        numWritten = write(destinationFd, buffer, numRead);
        // 0, 1, 2 has special meaning to replace destinationFd

        /* Check the partial write */
        if (numWritten < numRead)
        {
            printf("Partial write error %s\n", argv[2]);
            exit(EXIT_FAILURE);
        }

        /* Read the next chunk */
        numRead = read(sourceFd, buffer, CHUNK_SIZE); // read here, not write
    };



    /* close all files and check error */
    status = close(sourceFd);    

    if (status == -1)
    {
        printf("Fail to close file %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    status = close(destinationFd);
    if (status == -1)
    {
       printf("Fail to close file %s\n", argv[2]);
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}