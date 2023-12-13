#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
    char messageQueueName[50];
    int index, status, value, numMessages;
    unsigned int priority;
    mqd_t mqd;
    struct mq_attr attr, *attrp;

    /* TODO: Set the message queue name, maximum number of messages, and message size. */
    if (argc != 3 || atoi(argv[2]) < 1)
    {
        printf("Usage: %s <message queue name> <number of messages>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    numMessages = atoi(argv[2]); 

    snprintf(messageQueueName, sizeof(messageQueueName), "%s", argv[1]);

    attr.mq_maxmsg = numMessages;
    attr.mq_msgsize = sizeof(int);


    /* TODO: Create a message queue. */
    mqd = mq_open(
        messageQueueName,
        O_CREAT | O_WRONLY,
        S_IRUSR | S_IWUSR,
        &attr
    );

    if(mqd == (mqd_t) -1)
    {
        printf("Failed to create message queue.\n");
        exit(EXIT_FAILURE);
    }
    /* TODO: Write integers 1, 2, 3, ..., N to the message queue 
     * so that they will be read in Last In First Out (LIFO) order. 
     * Then close the message queue.
     */
    for (index = 1; index <= numMessages; index++)
    {
        status = mq_send(mqd, (char *)&index, sizeof(int), index);
        if (status == -1)
        {
            perror("Failed to send message to the queue");
            mq_close(mqd);
            mq_unlink(messageQueueName);
            exit(EXIT_FAILURE);
        }
    }

    mq_close(mqd);

    /* TODO: Open the message queue for reading. */
    mqd = mq_open(messageQueueName, O_RDONLY);
    if (mqd == (mqd_t) -1)
    {
        printf("Failed to open message queue.\n");
        exit(EXIT_FAILURE);
    }

    /* TODO: Read the messages from the message queue in Last In First Out (LIFO) order. */
    for (index = 0; index < numMessages; index++)
    {
        priority = numMessages - index;
        status = mq_receive(mqd, (char *)&value, sizeof(int), &priority);
        if (status == -1)
        {
            perror("Failed to receive message from the queue");
            mq_close(mqd);
            mq_unlink(messageQueueName);
            exit(EXIT_FAILURE);
        }

        /* TODO: Print the messages one message per line. */
        printf("%d\n", value);
    }
    /* TODO: Print the messages one message per line.
     * Then close and remove the message queue. 
     */

    mq_close(mqd);
    mq_unlink(messageQueueName);

    exit(EXIT_SUCCESS);
    
}
