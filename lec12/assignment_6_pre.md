## Assignment-6
- NUMBER OF QUESTIONS: 1
- TOTAL POINTS: 100
- TOTAL TIME: 30 minutes

Solve this assignment entirely on your own. Do not consult any books, websites or persons. Do not import "tlpi_hdr.h". Do not use any helper functions such as errExit( ) written by the textbook's author. Use of the "man" command is allowed.


This problem is about a writer process and a reader process communicating using a POSIX shared memory. A POSIX semaphore MUST be used.

## PROBLEM STATEMENT

Create two programs, `shmwriter.c` and `shmreader.c` as described below.

### `shmwriter.c`

This program should be compiled as follows:

`cc shmwriter.c -o shmwriter -lrt`

and run as follows:

`./shmwriter <shared memory name> <semaphore name> <n1> <n2> ...`

where <n1>, <n2>, ... are a positive integers.

For example, if the user runs:

`./shmwriter /SharedMemory /Semaphore 100 50 10`

then the program should create a new POSIX shared memory named /SharedMemory and write integers 100, 50, 10 to it, using a POSIX semaphore named /Semaphore (which must be created first) for accessing critical section.

If the number of command line arguments is not correct or if the user types

`./shmwriter --help`

or

`./shmwriter -h`

then the program must print

`Usage: ./shmwriter <shared memory name> <semaphore name> <n1> <n2> ...`

### `shmreader.c`

This program should be compiled as follows:

`cc shmreader.c -o shmreader -lrt`

and run as follows:

`./shmreader <shared memory name> <semaphore name>`

For example, if the user runs the following (after running ./shmwriter /SharedMemory /Semaphore 100 50 10)

`./shmreader /SharedMemory /Semaphore`

then the program should read the shared memory and print

100
50
10

using the POSIX semaphore named /Semaphore for accessing critical section.
If the number of command line arguments is not correct or if the user types

`./shmreader --help`

or

`./shmreader -h`

then the program must print

`Usage: ./shmreader <shared memory name> <semaphore name>`

WHAT TO SUBMIT

Submit your programs as two files named shmwriter.txt and shmreader.txt
Assignment-