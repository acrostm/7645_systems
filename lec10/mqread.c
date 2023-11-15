#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <fcntl.h>

typedef struct
{
    char name[20];
    int count;
} Person;

int main(int argc, char **argv)
{
    Person person;
    int numRead;
    mqd_t messageQueueDescriptor;
    struct mq_attr attributes;
    int status;
    unsigned priority;

    messageQueueDescriptor = mq_open(
        "/AwesomeMessageQueue",
        O_RDONLY);
    if (messageQueueDescriptor == (mqd_t)-1)
    {
        perror("Message queue creation");
        exit(EXIT_FAILURE);
    }

    status = mq_getattr(messageQueueDescriptor, &attributes);
    if (status == -1)
    {
        perror("Message queue get attributes");
        exit(EXIT_FAILURE);
    }

    printf("Maximum messages size:  %ld bytes\n", attributes.mq_msgsize);
    printf("Maximum number of message: %ld\n", attributes.mq_maxmsg);
    printf("Number of messages currently on queue: %ld\n", attributes.mq_curmsgs);

    status = mq_open(
        "/AwesomeMessageQueue",
        O_RDONLY);
    if (status == -1)
    {
        perror("Message queue creation");
        exit(EXIT_FAILURE);
    }

    numRead = mq_receive(
        messageQueueDescriptor,
        (char *)&person,
        sizeof(Person),
        &priority);

    while (numRead > 0)
    {
        printf("Name: %s\n", person.name);
        printf("Count: %d\n", person.count);
        printf("Priority: %u\n", priority);
    }

    if (numRead == -1)
    {
        perror("Message queue receive");
        exit(EXIT_FAILURE);
    }

    status = mq_close(messageQueueDescriptor);
    if (status == -1)
    {
        perror("Message queue close");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}