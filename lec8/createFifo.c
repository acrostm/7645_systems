/*
 * Create a new FIFO
 *
 * */

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char* argv[])
{
    int status;

    /* S_IXUSR, S_IXGRP, S_IXOTH should never be used here */
    status = mkfifo("MyFIFO", S_IRUSR | S_IWUSR | S_IRGRP);
    if (status == -1)
    {
        printf("Could not create FIFO.\n");
        exit(EXIT_FAILURE);
    }
    
    exit(EXIT_SUCCESS);
}