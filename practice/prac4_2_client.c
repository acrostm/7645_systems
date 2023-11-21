/* The client sending string to the FIFO and to the server to transform it */
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
    
}