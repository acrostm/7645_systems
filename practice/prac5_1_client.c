/* The client randomly create 10 double digits to the FIFO and to the server */
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

/* Maximum size of response FIFO name */
#define MAX_LENGTH 100

typedef struct
{
    pid_t clientId;
    double inputArray[10];
} Request;

typedef struct
{
    double average;
} Response;

/* define the FIFO name */
#define SERVER_FIFO_NAME "RequestFIFO"
#define RESPONSE_FIFO_TEMPLATE "ResponseFIFO.%d"

void removeResponseFifo(void)
{
    int status;
    char responseFifoName[MAX_LENGTH];
    snprintf(
            responseFifoName,
            MAX_LENGTH,
            RESPONSE_FIFO_TEMPLATE,
            getpid()
    );

    status = unlink(responseFifoName);
    if(status == -1)
    {
        printf("Could not remove FIFO %s\n", responseFifoName);
        _exit(EXIT_FAILURE); /* avoid infinite loop of calling exit handler */
    }
}

int main(int argc, char** argv)
{
    int status;
    int writeDescriptor, readDescriptor;
    int numRead, numWritten;
    Request req;
    Response res;
    char responseFifoName[MAX_LENGTH];

    /* Register exit handler - remove FIFO when exit the server */
    status = atexit(removeResponseFifo);
    if(status != 0)
    {
        printf("Could not register exit handler\n");
        exit(EXIT_FAILURE);
    }

    /* Check command line parameters */
    if(argc > 1 || strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0)
    {
        printf("Usage: %s\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    req.clientId = getpid();
    srand(time(NULL));
    for(int i = 0; i < 10; i++)
    {
        double randomValue = (double)rand() / (double)RAND_MAX;
        req.inputArray[i] = randomValue;
        printf("Input floats: %lf\t", req.inputArray[i]);
    }


    snprintf(
            responseFifoName,
            MAX_LENGTH,
            RESPONSE_FIFO_TEMPLATE,
            req.clientId
    );

    status = mkfifo(
            responseFifoName,
            S_IRUSR | S_IWUSR | S_IWGRP
    );
    if(status == -1)
    {
        printf("Client could not create FIFO %s\n", responseFifoName);
        exit(EXIT_FAILURE);
    }

    writeDescriptor = open(SERVER_FIFO_NAME, O_WRONLY);
    if(writeDescriptor == -1)
    {
        printf("Client could not find FIFO %s, no server running\n", SERVER_FIFO_NAME);
        exit(EXIT_FAILURE);
    }

    numWritten = write(writeDescriptor, &req, sizeof(Request));
    if(numWritten < sizeof(Request))
    {
        printf("Client could not write to FIFO %s\n", SERVER_FIFO_NAME);
        exit(EXIT_FAILURE);
    }

    /* close the write descriptor */
    status = close(writeDescriptor);
    if(status == -1)
    {
        printf("Client could not close write request FIFO %s\n", SERVER_FIFO_NAME);
        exit(EXIT_FAILURE);
    }

    /* Receive the response from the server */
    /* Open the read descriptor */
    readDescriptor = open(responseFifoName, O_RDONLY);
    if(readDescriptor == -1)
    {
        printf("Client could not open FIFO %s\n", responseFifoName);
        exit(EXIT_FAILURE);
    }

    /* Read the response */
    numRead = read(readDescriptor, &res, sizeof(Response));
    if(numRead < sizeof(Response))
    {
        printf("Client could not read from FIFO %s\n", responseFifoName);
        exit(EXIT_FAILURE);
    }

    /* Close the response read descriptor */
    status = close(readDescriptor);
    if(status == -1)
    {
        printf("Client could not close read response FIFO %s\n", responseFifoName);
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < 10; ++i) {
        printf("Input floats: %lf\t", req.inputArray[i]);
    }
    printf("Output average: %lf\n", res.average);

    exit(EXIT_SUCCESS);

}
