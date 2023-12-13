#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/types.h>
#include <semaphore.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int shmDescriptor;
    int status;
    void *addrToSharedMemory;
    int *pointerToSharedVariable;
    sem_t *mySemaphore;
    char *shmName = argv[1];
    char *semName = argv[2];
    struct stat shmMetadata;

    if (argc != 3 || strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0)
    {
        printf("Usage: %s <shared memory name> <semaphore name>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    mySemaphore = sem_open(semName, 0);
    if (mySemaphore == SEM_FAILED)
    {
        printf("Failed to open semaphore.\n");
        exit(EXIT_FAILURE);
    }

    shmDescriptor = shm_open(shmName, O_RDONLY, 0);
    if (shmDescriptor == -1)
    {
        printf("Failed to open shared memory.\n");
        exit(EXIT_FAILURE);
    }

    status = fstat(shmDescriptor, &shmMetadata);
    if (status == -1)
    {
        printf("Failed to get size of the shared memory.\n");
        exit(EXIT_FAILURE);
    }

    addrToSharedMemory = mmap(
        NULL, shmMetadata.st_size, PROT_READ, MAP_SHARED, shmDescriptor, 0);
    if (addrToSharedMemory == MAP_FAILED)
    {
        printf("Failed to map shared memory into address space.\n");
        exit(EXIT_FAILURE);
    }

    status = close(shmDescriptor);
    if (status == -1)
    {
        printf("Failed to close shared memory.\n");
        exit(EXIT_FAILURE);
    }

    pointerToSharedVariable = (int *)addrToSharedMemory;

    status = sem_wait(mySemaphore);
    if (status != 0)
    {
        printf("Failed to lock semaphore.\n");
        exit(EXIT_FAILURE);
    }

    int ans = 0;
    for (int i = 0; i < shmMetadata.st_size / sizeof(int); i++)
    {
        ans += pointerToSharedVariable[i];
    }

    printf("%d\n", ans);

    status = sem_post(mySemaphore);
    if (status == -1)
    {
        printf("Failed to unlock semaphore.\n");
        exit(EXIT_FAILURE);
    }

    status = sem_close(mySemaphore);
    if (status == -1)
    {
        printf("Failed to close semaphore.\n");
        exit(EXIT_FAILURE);
    }

    status = shm_unlink(shmName);
    if (status == -1)
    {
        printf("Failed to unlink shared memory.\n");
        exit(EXIT_FAILURE);
    }

    status = sem_unlink(semName);
    if (status == -1)
    {
        printf("Failed to unlink semaphore.\n");
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}
