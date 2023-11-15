#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>

typedef struct {
    char name[20];
    int count;
} Person;

int main(int argc, char** argv)
{
    mqd_t messageQueueDescriptor;
    int status;
    struct mq_attr attributes;

    /* MAX size of a message */
    attributes.mq_msgsize = sizeof(Person);
    
    /* MAX number of a message in the message queue */
    attributes.mq_maxmsg = 5;

    messageQueueDescriptor = mq_open(
        "/AwesomeMessageQueue",
        O_CREAT,
        S_IRUSR | S_IWUSR | S_IRGRP,
        &attributes
    );

    if (messageQueueDescriptor == (mqd_t) -1) {
        perror("Message queue creation");
        exit(EXIT_FAILURE);
    }

    return 0;
}