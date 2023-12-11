/* Compile using the -lrt option */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <mqueue.h>
#include <string.h>

#define MAX_MESSAGES 10
#define MAX_MESSAGE_SIZE 50

typedef struct {
    int number;
    int priority;
} Number;

int main(int argc, char** argv)
{
    int status;
    mqd_t messageQueueDescriptor;
    struct mq_attr attributes;
    char *mq_name = argv[1];
    Number *numbers;

    if (argc < 4 || strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
        printf("Usage: %s <message queue name> <n1> <p1> <n2> <p2> ...\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    attributes.mq_msgsize = MAX_MESSAGE_SIZE;
    attributes.mq_maxmsg = MAX_MESSAGES;

    messageQueueDescriptor = mq_open(
        mq_name,
        O_CREAT | O_WRONLY,
        S_IRUSR | S_IWUSR,
        &attributes
    );

    if(messageQueueDescriptor == (mqd_t) -1)
    {
        printf("Failed to open message queue.\n");
        exit(EXIT_FAILURE);
    }
    numbers = (Number *)malloc((argc - 2) / 2 * sizeof(Number));
    if (numbers == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    for(int i = 2, j = 0; i < argc; i += 2, j++)
    {   
        numbers[j].number = atoi(argv[i]);
        numbers[j].priority = atoi(argv[i + 1]);

        status = mq_send(
            messageQueueDescriptor,
            (char*)&numbers[j],
            sizeof(Number),
            numbers[j].priority
        );
        if(status == -1)
        {
            perror("mq_send");
            free(numbers);  
            exit(EXIT_FAILURE);
        }
    }

    status = mq_close(messageQueueDescriptor);
    if(status == -1)
    {
        printf("Failed to close message queue.\n");
        exit(EXIT_FAILURE);
    }

    free(numbers);
    exit(EXIT_SUCCESS);
}