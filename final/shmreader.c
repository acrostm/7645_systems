/*
 * Read the value of the shared variable 
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */
#include <sys/mman.h>        /* For mmap */  
#include <unistd.h>          /* For ftruncate and close */
#include <sys/types.h>       /* For ftruncate */
#include <semaphore.h>       /* For semaphores */
#include <string.h>          /* For strcmp */

int main(int argc, char* argv[])
{
    int shmDescriptor;
    int status;
    void* addrToSharedMemory;
    int* pointerToSharedVariable;
    sem_t* mySemaphore;
    char *shmName = argv[1];
    char *semName = argv[2];
    struct stat shmMetadata;

    if(argc != 3 || strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0){
        printf("Usage: %s <shared memory name> <semaphore name>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    /* Open the existing semaphore */
    mySemaphore = sem_open(semName, 0);
    if(mySemaphore == SEM_FAILED){
        printf("Failed to open semaphore.\n");
        exit(EXIT_FAILURE);
    }

    /* Open the shared variable for reading */
    shmDescriptor = shm_open(shmName, O_RDONLY, 0);
    if(shmDescriptor == -1){
        printf("Failed to open shared memory.\n");
        exit(EXIT_FAILURE);
    }

    /* Get metadata about the shared variable */
    status = fstat(shmDescriptor, &shmMetadata);
    if(status == -1){
        printf("Failed to get size of the shared memory.\n");
        exit(EXIT_FAILURE);
    }

    /* Map this shared memory into my address space */
    addrToSharedMemory = mmap(
        NULL, shmMetadata.st_size, PROT_READ, MAP_SHARED, shmDescriptor, 0);
    if(addrToSharedMemory == MAP_FAILED){
        printf("Failed to map shared memory into address space.\n");
        exit(EXIT_FAILURE);
    }

    status = close(shmDescriptor);
    if(status == -1){
        printf("Failed to close shared memory.\n");
        exit(EXIT_FAILURE);
    }

    /* Get a pointer to the shared variable */
    pointerToSharedVariable = (int*) addrToSharedMemory;

    status = sem_wait(mySemaphore);
    if(status != 0){
        printf("Failed to lock semaphore.\n");
        exit(EXIT_FAILURE);
    }

    // CRITICAL SECTION
    int ans = 0;
    ans = atoi(argv[3]) + atoi(argv[4]);
    printf("%d\n", ans);

    /* Unlock the semaphore */
    status = sem_post(mySemaphore);
    if(status != 0){
        printf("Failed to unlock semaphore.\n");
        exit(EXIT_FAILURE);
    }

    /* Close the semaphore */
    status = sem_close(mySemaphore);
    if(status != 0){
        printf("Failed to close semaphore.\n");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
