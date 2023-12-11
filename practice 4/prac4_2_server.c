/* The server of FIFO, take the string from client and transform into Capital */
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <ctype.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>

/* Maximum size of response FIFO name */
#define MAX_LENGTH 100

typedef struct
{
    pid_t clientId;
    char inputString[MAX_LENGTH];
} Request;

typedef struct
{
    char outputString[MAX_LENGTH];
} Response;

/* define the FIFO name */
#define SERVER_FIFO_NAME "RequestFIFO"
#define RESPONSE_FIFO_TEMPLATE "ResponseFIFO.%d"

#define TRUE 1
#define FALSE 0

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

void* transformString(void* ptrToRequest)
{
    Response res;
    int status;
    int writeDescriptor, numWritten;
    char responseFifoName[MAX_LENGTH];
    Request req = *(Request *)ptrToRequest;

    printf(
            "Getting string from client#%d: %s \n",
            req.clientId, req.inputString
            );

    status = pthread_mutex_lock(&mutex);
    if(status != 0)
    {
        printf("Could not lock mutex\n");
        exit(EXIT_FAILURE);
    }

    for(int i = 0; i < strlen(req.inputString); i++)
    {
        res.outputString[i] = toupper(req.inputString[i]);
    }

    status = pthread_mutex_unlock(&mutex);
    if(status != 0)
    {
        printf("Could not unlock mutex\n");
        exit(EXIT_FAILURE);
    }

    snprintf(
            responseFifoName,
            MAX_LENGTH,
            RESPONSE_FIFO_TEMPLATE,
            req.clientId
            );

    writeDescriptor = open(responseFifoName, O_WRONLY);
    if(writeDescriptor == -1)
    {
        printf("Could not open FIFO %s\n", responseFifoName);
        exit(EXIT_FAILURE);
    }

    numWritten = write(
            writeDescriptor,
            &res,
            sizeof(Response)
            );
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

    free(ptrToRequest);

    pthread_exit(NULL);


}

int main(int argc, char* argv[])
{
    Request req;
    int status, numRead;
    int readDescriptor, dummyDescriptor;
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
            S_IRUSR | S_IWUSR | S_IWGRP
            );
    if (status == -1)
    {
        printf("Could not create FIFO %s\n", SERVER_FIFO_NAME);
        exit(EXIT_FAILURE);
    }

    readDescriptor = open(SERVER_FIFO_NAME, O_RDONLY);
    if(readDescriptor == -1)
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

    while(TRUE)
    {
        numRead = read(readDescriptor, &req, sizeof(Request));
        if(numRead == -1)
        {
            printf("Could not read from FIFO %s\n", SERVER_FIFO_NAME);
            exit(EXIT_FAILURE);
        }

        /* Create a brand-new request object just for the child thread */
        threadInput = (Request *)malloc(sizeof(Request));
        if (threadInput == NULL)
        {
            printf("Could not allocate memory for thread input\n");
            exit(EXIT_FAILURE);
        }
        memcpy(threadInput, &req, sizeof(Request));

        /* use a new thread to handle write to the response fifo */
        status = pthread_create(
                &requestHandlerThread,
                NULL,
                transformString,
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