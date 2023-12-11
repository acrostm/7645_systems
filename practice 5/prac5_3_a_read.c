/* Compile using the -lrt option */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <mqueue.h>

#define MAX_MESSAGES 10
#define MAX_MESSAGE_SIZE 50

typedef struct {
    char firstName[20];
    char lastName[20];
} Person;

int main(int argc, char** argv)
{
    int status, numRead;
    mqd_t messageQueuDescriptor;
    struct mq_attr attributes;
    unsigned int priority;
    Person person;

    messageQueuDescriptor = mq_open(
        "/MyMessageQueue",
        O_RDONLY
    );
    if(messageQueuDescriptor == (mqd_t) -1)
    {
        printf("Failed to open message queue.\n");
        exit(EXIT_FAILURE);
    }

    status = mq_getattr(messageQueuDescriptor, &attributes);
    printf("Maximum size of a message = %ld.\n", attributes.mq_msgsize);
    printf("Maximum number of messages = %ld.\n", attributes.mq_maxmsg);
    printf("Current number of messages = %ld.\n", attributes.mq_curmsgs);

    numRead = mq_receive(
        messageQueuDescriptor,
        (char*)&person,
        sizeof(Person),
        &priority
    );

    while(numRead > 0)
    {
        printf(
            "Name = %s %s, priority = %u.\n",
            person.firstName,
            person.lastName,
            priority
        );

        numRead = mq_receive(
            messageQueuDescriptor,
            (char*)&person,
            sizeof(Person),
            &priority
        );
    }

    if (numRead == -1)
    {
        printf("Failed to read from message queue.\n");
        exit(EXIT_FAILURE);
    }

    status = mq_close(messageQueuDescriptor);
    if(status == -1)
    {
        printf("Failed to close message queue.\n");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}