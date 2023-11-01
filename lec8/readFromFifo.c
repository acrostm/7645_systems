/* Read the data in the FIFO pipe now */
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char** argv)
{
    int fifoDescriptor;
    int status;
    int numRead;
    int value;

    fifoDescriptor = open("MyFIFO", O_RDONLY);
    if (fifoDescriptor == -1)
    {
        printf("Could not open \"MyFIFO\".\n");
        exit(EXIT_FAILURE);
    }

    printf("Opened \"MyFIFO\" for reading.\n");

    numRead = read(fifoDescriptor, &value, sizeof(int));
    if (numRead == -1)
    {
        printf("Could not read from \"MyFIFO\".\n");
        exit(EXIT_FAILURE);
    }

    printf("Read %d bytes from the FIFO.\n", numRead);

    while (numRead > 0)
    {
        printf("Read value: %d\n", value);
        numRead = read(fifoDescriptor, &value, sizeof(int));
        if (numRead == -1)
        {
            printf("Could not read from \"MyFIFO\".\n");
            exit(EXIT_FAILURE);
        }
        
    }

    status = close(fifoDescriptor);
    if (status == -1)
    {
        printf("Could not close \"MyFIFO\".\n");
        exit(EXIT_FAILURE);
    }
}