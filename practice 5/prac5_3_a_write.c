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
    int status;
    mqd_t messageQueueDescriptor;
    struct mq_attr attributes;

    attributes.mq_msgsize = MAX_MESSAGE_SIZE;
    attributes.mq_maxmsg = MAX_MESSAGES;
    
    Person persons[5] = {
        {"Aice", "Anderson"},
        {"Bob", "Brown"},
        {"Cathy", "Clark"},
        {"Daniel", "Davis"},
        {"Ethan", "Evans"}
    };

    messageQueueDescriptor = mq_open(
        "/MyMessageQueue",
        O_CREAT | O_WRONLY,
        S_IRUSR | S_IWUSR,
        &attributes
    );

    if(messageQueueDescriptor == (mqd_t) -1)
    {
        printf("Failed to open message queue.\n");
        exit(EXIT_FAILURE);
    }

    for(int i = 0; i < 5; i++)
    {
        status = mq_send(
            messageQueueDescriptor,
            (char*)&persons[i],
            sizeof(Person),
            i
        );
        if(status == -1)
        {
            printf("Failed to write to message queue.\n");
            exit(EXIT_FAILURE);
        }
    }

    status = mq_close(messageQueueDescriptor);
    if(status == -1)
    {
        printf("Failed to close message queue.\n");
        exit(EXIT_FAILURE);
    }


    exit(EXIT_SUCCESS);
}