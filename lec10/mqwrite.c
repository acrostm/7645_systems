#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <fcntl.h>

typedef struct {
    char name[20];
    int count;
} Person;

int main(int argc, char** argv)
{
    Person persons[5] = {
        {"Alice", 50},
        {"Bob", 40},
        {"Cathy", 30},
        {"Daniel", 20},
        {"Ethan", 10}
    };

    mqd_t messageQueueDescriptor;
    int status;

    messageQueueDescriptor = mq_open(
        "/AwesomeMessageQueue",
        O_WRONLY
    );
    if (messageQueueDescriptor == (mqd_t) -1) {
        perror("Message queue creation");
        exit(EXIT_FAILURE);
    }

    for(int i = 0; i < 5; i++)
    {
        status = mq_send(
            messageQueueDescriptor,
            (char*) &persons[i],
            sizeof(Person),
            (unsigned) i
        );
        if(status == -1)
        {
            perror("Message queue send");
            exit(EXIT_FAILURE);
        }
    }
    
    status = mq_close(messageQueueDescriptor);
    if(status == -1)
    {
        perror("Message queue close");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}