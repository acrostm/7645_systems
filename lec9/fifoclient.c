#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

typedef struct 
{
    pid_t clientId;
    unsigned numTickets;
} Request;

typedef struct 
{
    unsigned firstTicket;
    unsigned lastTicket;
} Response;

/* define the FIFO name */
#define SERVER_FIFO_NAME "RequestFIFO"
#define RESPONSE_FIFO_TEMPLATE "ResponseFIFO.%d"

/* Maximum size of response FIFO name */
#define MAX_LENGTH 100

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
    int numTickets;
    int status;
    int writeDescriptor, readDescriptor; /* no dummy, because the client dont have to be keeping open */
    int numRead, numWritten;
    Request request;
    Response response;
    char responseFifoName[MAX_LENGTH];

    /* Register exit handler - remove FIFO when exit the server */
    status = atexit(removeResponseFifo);
    if(status != 0)
    {
        printf("Could not register exit handler\n");
        exit(EXIT_FAILURE);
    }

    /* Check command line parameters */
    if(argc != 2 || strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0)
    {
        printf("Usage: %s <numTickets>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    numTickets = atoi(argv[1]);
    if(numTickets <= 0)
    {
        printf("Number of tickets must be positive\n");
        exit(EXIT_FAILURE);
    }

    /* Put together the request*/
    request.clientId = getpid();
    request.numTickets = (unsigned) numTickets; /* turn int to unsigned after validating it */

    /* Create the response FIFO (named pipe) */
    snprintf(
        responseFifoName,
        MAX_LENGTH,
        RESPONSE_FIFO_TEMPLATE,
        request.clientId
    );
    status = mkfifo(
        responseFifoName,
        S_IRUSR | S_IWUSR | S_IWGRP
    );
    if (status == -1)
    {
        printf("Client could not create FIFO %s\n", responseFifoName);
        exit(EXIT_FAILURE);
    }

    /* Send the request to the server */
    writeDescriptor = open(SERVER_FIFO_NAME, O_WRONLY);
    if(writeDescriptor == -1)
    {
        printf("Client could not find FIFO %s, no server running\n", SERVER_FIFO_NAME);
        exit(EXIT_FAILURE);
    }

    numWritten = write(writeDescriptor, &request, sizeof(Request));
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
    numRead = read(readDescriptor, &response, sizeof(Response));
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

    /* Display the response */
    printf("First ticket: %d\n", response.firstTicket);
    printf("Last ticket: %d\n", response.lastTicket);

    exit(EXIT_SUCCESS);
}