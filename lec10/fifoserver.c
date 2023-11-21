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

/* multi thread must has access to the next ticket number at the same time */
volatile unsigned nextTicket = 1;

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

void *giveOutTickets(void *requestPointer)
{
    Response response;
    int status;
    int writeDescriptor, numWritten;
    char responseFifoName[MAX_LENGTH];
    Request request = *(Request *)requestPointer;

    printf(
        "Giving out tickets to clientID %d for %u tickets\n",
        request.clientId,
        request.numTickets);

    /* Compute the response */
    status = pthread_mutex_lock(&mutex);
    if (status != 0)
    {
        printf("Could not lock mutex\n");
        exit(EXIT_FAILURE);
    }

    response.firstTicket = nextTicket;
    response.lastTicket = response.firstTicket + request.numTickets - 1;
    nextTicket = response.lastTicket + 1;

    status = pthread_mutex_unlock(&mutex);
    if (status != 0)
    {
        printf("Could not unlock mutex\n");
        exit(EXIT_FAILURE);
    }

    /* Send the response */
    /* Calculate the name of the Response FIFO */
    snprintf(
        responseFifoName,
        MAX_LENGTH,
        RESPONSE_FIFO_TEMPLATE,
        request.clientId);

    /* Open the Response FIFO */
    writeDescriptor = open(responseFifoName, O_WRONLY);
    if (writeDescriptor == -1)
    {
        printf("Could not open FIFO %s\n", responseFifoName);
        exit(EXIT_FAILURE);
    }

    /* Write the response */
    numWritten = write(
        writeDescriptor,
        &response,
        sizeof(Response));
    if (numWritten < sizeof(Response))
    {
        printf("FIFO %s has partial write issue\n", responseFifoName);
        exit(EXIT_FAILURE);
    }

    /* Close the Response FIFO */
    status = close(writeDescriptor);
    if (status == -1)
    {
        printf("Could not close FIFO %s\n", responseFifoName);
        exit(EXIT_FAILURE);
    }

    /* free the reuest */
    free(requestPointer);

    pthread_exit(NULL);
}

int main(int argc, char **argv)
{
    int status;
    int numRead;
    int readDescriptor, dummyDescriptor;
    Request request;
    Request *threadInput;
    pthread_t requestHandlerThread;

    /* Register exit handler - remove FIFO when exit the server */
    status = atexit(removeRequestFifo);
    if (status != 0)
    {
        printf("Could not register exit handler\n");
        exit(EXIT_FAILURE);
    }

    /* Register Ctrl - C signal handler - remove FIFO when abnormal exit the server */
    /* SIGINT - signal interrupt */
    if (signal(SIGINT, handleSignal) == SIG_ERR)
    {
        printf("Could not register signal handler\n");
        _exit(EXIT_FAILURE); /* avoid infinite loop of calling exit handler */
    }

    status = mkfifo(
        SERVER_FIFO_NAME,
        S_IRUSR | S_IWUSR | S_IWGRP);
    if (status == -1)
    {
        printf("Could not create FIFO %s\n", SERVER_FIFO_NAME);
        exit(EXIT_FAILURE);
    }

    readDescriptor = open(SERVER_FIFO_NAME, O_RDONLY);
    if (readDescriptor == -1)
    {
        printf("Could not open FIFO read descriptor %s\n", SERVER_FIFO_NAME);
        exit(EXIT_FAILURE);
    }

    dummyDescriptor = open(SERVER_FIFO_NAME, O_WRONLY);
    if (dummyDescriptor == -1)
    {
        printf("Could not open FIFO dummy descriptor %s\n", SERVER_FIFO_NAME);
        exit(EXIT_FAILURE);
    }

    while (TRUE) /* can also be written as for(;;) */
    {
        /* Read the request */

        numRead = read(readDescriptor, &request, sizeof(Request));
        if (numRead == -1)
        {
            printf("Could not read from FIFO %s\n", SERVER_FIFO_NAME);
            exit(EXIT_FAILURE);
        }

        /* Create a brand new request object just for the child thread */
        threadInput = (Request *)malloc(sizeof(Request));
        if (threadInput == NULL)
        {
            printf("Could not allocate memory for thread input\n");
            exit(EXIT_FAILURE);
        }
        memcpy(threadInput, &request, sizeof(Request));

        /* use a new thread to handle write to the response fifo */
        status = pthread_create(
            &requestHandlerThread,
            NULL,
            giveOutTickets,
            (void *)threadInput
        );

        if (status != 0)
        {
            printf("Could not create thread\n");
            exit(EXIT_FAILURE);
        }

        /* Detach the request handler,
            so that the main thread can move on the other threads,
            reading other requests
            ans this thread will call pthread_exit and its entry in the thread table
            without creating a zombie thread */
        status = pthread_detach(requestHandlerThread);
        if (status != 0)
        {
            printf("Could not detach thread\n");
            exit(EXIT_FAILURE);
        }
    }

    exit(EXIT_SUCCESS);
}
