/* ********************************
 * Consider two processes:  a producer and a consumer, and a shared variablenumMessages.  
 * The shared variable starts with an initial value of 0.  
 * The producer increases the value ofnumMessagesin steps of 1 in an infinite loop until it reaches a maximum value of 1024.  
 * When the producer tries to incrementthe value ofnumMessagesbeyond 1024, it must block until the value is less than 1024.  
 * The consumer decrements the value ofnumMessagesin steps of 1 in an infinite loop until it reaches a minimum valueof 0.  
 * When the consumer tries to decrement the value ofnumMessagesbelow 0, it must block untilthe value becomes positive.  
 * When the producer and consumer are run simultaneously, the followingconditions must be satisfied:
 * (a)  There must be no race conditions.
 * (b)numMessagesmust not exceed 1024.
 * (c)numMessagesmust not be negative.
 * **********************************
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>  /* For mode constants */
#include <fcntl.h>     /* For O_* constants */
#include <sys/mman.h>  /* For mmap */
#include <unistd.h>    /* For ftruncate and close */
#include <sys/types.h> /* For ftruncate */
#include <semaphore.h> /* For using semaphores */
#include <string.h>    /* For strcmp */

#define TRUE 1
#define FALSE 0

int main(int argc, char *argv[])
{
    int shmDescriptor;
    int status;
    void* addrToSharedMemory;
    int* pointerToSharedVariable;
    sem_t* mSemaphore;
    sem_t* numEmplty = 0;
    sem_t* numFull = 1024;

    
}