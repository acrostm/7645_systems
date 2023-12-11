#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <string.h>

#define MAX_MESSAGES 10
#define MAX_MESSAGE_SIZE 100

int main(int argc, char** argv) {
    int status, numRead;
    mqd_t messageQueueDescriptor;
    struct mq_attr attributes;
    char *mq_name = argv[1];
    unsigned int priority;

    if (argc != 2 || strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
        printf("Usage: %s <message queue name>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    messageQueueDescriptor = mq_open(mq_name, O_RDONLY);
    if (messageQueueDescriptor == (mqd_t)-1) {
        perror("mq_open");
        exit(EXIT_FAILURE);
    }

    status = mq_getattr(messageQueueDescriptor, &attributes);
    if (status == -1) {
        perror("mq_getattr");
        exit(EXIT_FAILURE);
    }

    int msg;
    numRead = mq_receive(messageQueueDescriptor, (char *)&msg, MAX_MESSAGE_SIZE, &priority);
    printf("Received %d messages\n", numRead);
    while (numRead > 0) {
        printf("%d ", msg);
        numRead = mq_receive(messageQueueDescriptor, (char *)&msg, MAX_MESSAGE_SIZE, &priority);
        if (numRead == -1) {
            perror("mq_receive");
            exit(EXIT_FAILURE);
        }
    }
    
    printf("\n");

    status = mq_close(messageQueueDescriptor);
    if (status == -1) {
        perror("mq_close");
        exit(EXIT_FAILURE);
    }

    status = mq_unlink(mq_name);
    if (status == -1) {
        perror("mq_unlink");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
