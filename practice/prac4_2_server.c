/* The server of FIFO, take the string from client and transform into Capital */
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>

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

#define TRUE 1
#define FALSE 0

/* define the FIFO name */
#define SERVER_FIFO_NAME "RequestFIFO"
#define RESPONSE_FIFO_TEMPLATE "ResponseFIFO.%d"

/* Maximum size of response FIFO name */
#define MAX_LENGTH 100

/* mutex */
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

/* function for cleaning up by removing the RequestFIFO */
void removeRequestFifo(void)
{
    int status;
    status = unlink(SERVER_FIFO_NAME);
    if (status == -1)
    {
        printf("Could not remove FIFO %s\n", SERVER_FIFO_NAME);
        exit(EXIT_FAILURE);
    }
}

void handleSignal(int)
{
    printf("\n");
    exit(EXIT_SUCCESS);
}

int main(int argc, char* argv[])
{
    char input[100] = argv[1];
    Request req;
    Response res;
    
}