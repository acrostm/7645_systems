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
    int status, index, numRead;
    mqd_t messageQueueDescriptor;
    struct mq_attr attributes;
    unsigned priority;
    Person person;

    messageQueueDescriptor = mq_open(
        "/MyFirstMessageQueue",
        O_RDONLY
    );

    if (messageQueueDescriptor == (mqd_t) -1)
    {
        printf("Failed to open message queue.\n");
        exit(EXIT_FAILURE);
    }

    status = mq_getattr(messageQueueDescriptor, &attributes);
    printf("Maximum size of a message = %ld.\n", attributes.mq_msgsize);
    printf("Maximum number of messages = %ld.\n", attributes.mq_maxmsg);
    printf("Current number of messages = %ld.\n", attributes.mq_curmsgs);

    numRead = mq_receive(
        messageQueueDescriptor,
        (char*)&person,
        sizeof(Person),
        &priority
    );

    while (numRead > 0)
    {
        printf(
            "Name = %s, count = %d, priority = %u.\n",
            person.name,
            person.count,
            priority
        );

        numRead = mq_receive(
            messageQueueDescriptor,
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
    
    status = mq_close(messageQueueDescriptor);
    if (status == -1)
    {
        printf("Failed to close message queue.\n");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
