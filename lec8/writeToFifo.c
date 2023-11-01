/* Write the integer 5, 10, 15, 20 to a FIFO */
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char** argv)
{
    int numWritten;
    int fifoDescriptor;
    int status;
    int size;

    int numbersToWrite[4] = {5, 10, 15, 20};

    fifoDescriptor = open("MyFIFO", O_WRONLY);
    if (fifoDescriptor == -1)
    {
        printf("Could not open \"MyFIFO\".\n");
        exit(EXIT_FAILURE);
    }
    printf("Opened \"MyFIFO\" for writing.\n");
    size = 4 * sizeof(int);
    numWritten = write(fifoDescriptor, numbersToWrite, size);
    if(numWritten != size)
    {
        printf("Partial write error to the FIFO.\n");
        exit(EXIT_FAILURE);
    }
    printf("Wrote %d bytes to the FIFO.\n", numWritten);
    status = close(fifoDescriptor);
    if (status == -1)
    {
        printf("Could not close \"MyFIFO\".\n");
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
    
}
