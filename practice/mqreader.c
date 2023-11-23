#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <string.h>
#define MAX_MESSAGES 100
#define MAX_MESSAGE_SIZE 500
typedef struct
{

    int number;

    int priority;
} Number;
int main(int argc, char **argv)
{

    int status, numRead;

    mqd_t messageQueuDescriptor;

    struct mq_attr attributes;

    unsigned int priority;

    Number numbers;

    char *mq_name = argv[1];

    if (argc != 2 || strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0)
    {

        printf("Usage: %s <message queue name>\n", argv[0]);

        exit(EXIT_FAILURE);
    }

    messageQueuDescriptor = mq_open(

        mq_name,

        O_RDONLY

    );

    if (messageQueuDescriptor == (mqd_t)-1)

    {

        perror("mq_open");

        exit(EXIT_FAILURE);
    }

    status = mq_getattr(messageQueuDescriptor, &attributes);

    if (status == -1)

    {

        perror("mq_getattr");

        exit(EXIT_FAILURE);
    }

    numRead = mq_receive(

        messageQueuDescriptor,

        (char *)&numbers,

        attributes.mq_msgsize,

        &numbers.priority

    );

    while (numRead > 0)

    {

        printf(

            "%d\n",

            numbers.number

        );

        numRead = mq_receive(

            messageQueuDescriptor,

            (char *)&numbers,

            1024,

            &numbers.priority

        );

        mq_getattr(messageQueuDescriptor, &attributes);
        if(attributes.mq_curmsgs == 0)

        {

            break;
        }
    }

    if (numRead == -1)

    {

        perror("mq_receive");

        exit(EXIT_FAILURE);
    }

    status = mq_close(messageQueuDescriptor);

    if (status == -1)

    {

        perror("mq_close");

        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
