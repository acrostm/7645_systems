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
    int index;

    Person persons[5] = {
        {"Aice", 10},
        {"Bob", 20},
        {"Cathy", 30},
        {"Daniel", 40},
        {"Ethan", 50}
    };
    
    messageQueueDescriptor = mq_open(
        "/MyFirstMessageQueue",
        O_WRONLY
    );

    if (messageQueueDescriptor == (mqd_t) -1)
    {
        printf("Failed to open message queue.\n");
        exit(EXIT_FAILURE);
    }

    for (index = 0; index < 5; index++)
    {
        status = mq_send(
            messageQueueDescriptor,
            (char*)&persons[index],
            sizeof(Person),
            index
        );

        if (status == -1)
        {
            printf("Failed to write to message queue.\n");
            exit(EXIT_FAILURE);
        }
    }

    status = mq_close(messageQueueDescriptor);
    if (status == -1)
    {
        printf("Failed to close message queue.\n");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
