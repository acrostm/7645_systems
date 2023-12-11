#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>  /* For mode constants */
#include <fcntl.h>     /* For O_* constants */
#include <sys/mman.h>  /* For mmap */
#include <unistd.h>    /* For ftruncate and close */
#include <sys/types.h> /* For ftruncate */
#include <semaphore.h> /* For using semaphores */
#include <string.h>    /* For strcmp */

int main(int argc, char *argv[])
{
    int shmDescriptor;
    int status;
    void* addrToSharedMemory;
    char* pointerToSharedVariable;
    sem_t* mySemaphore;
    char *shmName = argv[1];
    char *semName = argv[2];

    if(argc < 4 || strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0){
        printf("Usage: %s <shared memory name> <semaphore name> <s1> <s2> ...\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    mySemaphore = sem_open(semName, O_CREAT, S_IRUSR | S_IWUSR, 1);
    if(mySemaphore == SEM_FAILED){
        printf("Failed to create semaphore.\n");
        exit(EXIT_FAILURE);
    }

    shmDescriptor = shm_open(shmName, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if(shmDescriptor == -1){
        printf("Failed to create shared memory.\n");
        exit(EXIT_FAILURE);
    }

    status = ftruncate(shmDescriptor, sizeof(char) * 100 * (argc - 3));
    if(status == -1){
        printf("Failed to set the size of the shared memory.\n");
        exit(EXIT_FAILURE);
    }

    addrToSharedMemory = mmap(
        NULL, sizeof(char) * (argc - 3) * 100, PROT_READ | PROT_WRITE, MAP_SHARED, shmDescriptor, 0);
    if(addrToSharedMemory == MAP_FAILED){
        printf("Failed to map shared memory into address space.\n");
        exit(EXIT_FAILURE);
    }

    status = close(shmDescriptor);
    if(status == -1){
        printf("Failed to close shared memory.\n");
        exit(EXIT_FAILURE);
    }

    pointerToSharedVariable = (char*) addrToSharedMemory;

    status = sem_wait(mySemaphore);
    if(status != 0){
        printf("Failed to lock semaphore.\n");
        exit(EXIT_FAILURE);
    }

    for(int i = 0; i < argc - 3; i++){
        strcpy(pointerToSharedVariable, argv[i + 3]);
        pointerToSharedVariable += strlen(argv[i + 3]) + 1;
    }

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
