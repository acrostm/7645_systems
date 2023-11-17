/* Compile using the -lrt option */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <mqueue.h>


typedef struct
{
    char name[20];
    int count;
} Person;

int main(int argc, char* argv[])
{
    int status;
    mqd_t messageQueueDescriptor;
    struct mq_attr attributes;

    /* Define the MAXIMUM size of a message */
    attributes.mq_msgsize = sizeof(Person);
    
    /* Maximum number of messages the message queue can hold */
    attributes.mq_maxmsg = 5;

    messageQueueDescriptor = mq_open(
        "/MyFirstMessageQueue",
        O_CREAT,
        S_IRUSR | S_IWUSR,
        &attributes
    );

    if (messageQueueDescriptor == (mqd_t) -1)
    {
        printf("Failed to create message queue.\n");
        exit(EXIT_FAILURE);
    }

    status = mq_close(messageQueueDescriptor);
    if (status == -1)
    {
        printf("Failed to close message queue.\n");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
