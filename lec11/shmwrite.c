/*
 * Create a POSIX shared memory object whose name is /MySharedMemory
 * It should contain a single integer whose value is 5
 */

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
    void *addr;
    int *pointerToSharedVariable;
    sem_t *mySemaphore;

    /* Create the semaphore if it doesn't already exist */
    mySemaphore = sem_open("/MySemaphore", O_CREAT, S_IRUSR | S_IWUSR, 1);
    if (mySemaphore == SEM_FAILED)
    {
        printf("Failed to create semaphore.\n");
        exit(EXIT_FAILURE);
    }

    /* Open the shared variable (create if it doesn't exist)*/
    shmDescriptor = shm_open(
        "/MySharedMemory", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (shmDescriptor == -1)
    {
        printf("Failed to create shared memory.\n");
        exit(EXIT_FAILURE);
    }

    /* Set the size of the shared memory to the size of an integer */
    status = ftruncate(shmDescriptor, sizeof(int));
    if (status == -1)
    {
        printf("Failed to set the size of the shared memory.\n");
        exit(EXIT_FAILURE);
    }

    /* Map this shared memory into my address space */
    addr = mmap(
        NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shmDescriptor, 0);
    if (addr == MAP_FAILED)
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

    pointerToSharedVariable = (int *)addr;

    status = sem_wait(mySemaphore);
    if (status != 0)
    {
        printf("Failed to lock semaphore.\n");
        exit(EXIT_FAILURE);
    }
    *pointerToSharedVariable = 5; // CRITICAL SECTION
    status = sem_post(mySemaphore);
    if (status != 0)
    {
        printf("Failed to unlock semaphore.\n");
        exit(EXIT_FAILURE);
    }

    status = sem_close(mySemaphore);
    if (status != 0)
    {
        printf("Failed to close semaphore.\n");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}