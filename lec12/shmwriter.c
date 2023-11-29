#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>  /* For mode constants */
#include <fcntl.h>     /* For O_* constants */
#include <sys/mman.h>  /* For mmap */
#include <unistd.h>    /* For ftruncate and close */
#include <sys/types.h> /* For ftruncate */
#include <semaphore.h> /* For using semaphores */

int main(int argc, char *argv[])
{
    int shmDescriptor;
    int status;
    void* addrToSharedMemory;
    int* pointerToSharedVariable;
    sem_t* mySemaphore;
    char *shmName = argv[1];
    char *semName = argv[2];

    if(argc < 4 || strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0){
        printf("Usage: %s <shared memory name> <semaphore name> <n1> <n2> ...\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    /* Create the semaphore if it doesn't already exist */
    mySemaphore = sem_open(semName, O_CREAT, S_IRUSR | S_IWUSR, 1);
    if(mySemaphore == SEM_FAILED){
        printf("Failed to create semaphore.\n");
        exit(EXIT_FAILURE);
    }

    /* Open the shared variable (create if it doesn't exist) */
    shmDescriptor = shm_open(shmName, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if(shmDescriptor == -1){
        printf("Failed to create shared memory.\n");
        exit(EXIT_FAILURE);
    }

    /* Set the size of the shared memory to the size of an integer */
    status = ftruncate(shmDescriptor, sizeof(int) * (argc - 3));
    if(status == -1){
        printf("Failed to set the size of the shared memory.\n");
        exit(EXIT_FAILURE);
    }

    /* Map this shared memory into my address space */
    addrToSharedMemory = mmap(
        NULL, sizeof(int) * (argc - 3), PROT_READ | PROT_WRITE, MAP_SHARED, shmDescriptor, 0);
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

    /* lock semaphore */
    status = sem_wait(mySemaphore);
    if(status != 0){
        printf("Failed to lock semaphore.\n");
        exit(EXIT_FAILURE);
    }

    /* CRITICAL SECTION */
    for(int i = 0; i < argc - 3; i++){
        pointerToSharedVariable[i] = atoi(argv[i + 3]);
    }

    /* unlock semaphore */
    status = sem_post(mySemaphore);
    if(status != 0){
        printf("Failed to unlock semaphore.\n");
        exit(EXIT_FAILURE);
    }

    status = sem_close(mySemaphore);
    if(status != 0){
        printf("Failed to close semaphore.\n");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}