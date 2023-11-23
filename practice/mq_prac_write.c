#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <string.h>

#define MAX_MESSAGES 10
#define MAX_MESSAGE_SIZE 100

int main(int argc, char** argv) {
    int status;
    mqd_t messageQueueDescriptor;
    struct mq_attr attributes;
    char *mq_name = argv[1];
    int n = atoi(argv[2]);

    if (argc != 3 || strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
        printf("Usage: %s <message queue name> <n>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    attributes.mq_maxmsg = MAX_MESSAGES;
    attributes.mq_msgsize = MAX_MESSAGE_SIZE;

    messageQueueDescriptor = mq_open(
        mq_name,
        O_CREAT | O_WRONLY,
        S_IRUSR | S_IWUSR,
        &attributes
    );

    if (messageQueueDescriptor == (mqd_t)-1) {
        perror("mq_open");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < n; i++) {
        status = mq_send(
            messageQueueDescriptor,
            (char*)&i,
            sizeof(int),
            i
        );
        if (status == -1) {
            perror("mq_send");
            exit(EXIT_FAILURE);
        }
    }

    status = mq_close(messageQueueDescriptor);
    if (status == -1) {
        perror("mq_close");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
