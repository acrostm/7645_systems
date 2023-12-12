#include <sys/un.h>
#include <sys/socket.h>
#include <ctype.h>
#include <sys/types.h>  /* Type definitions used by many programs */
#include <stdio.h>      /* Standard I/O functions */
#include <stdlib.h>     /* Prototypes of commonly used library functions,
                           plus EXIT_SUCCESS and EXIT_FAILURE constants */
#include <unistd.h>     /* Prototypes for many system calls */
#include <string.h>     /* Commonly used string-handling functions */
#include <errno.h>      /* Declares errno and defines error constants */

#define BUF_SIZE 100     /* Maximum size of messages exchanged between client and server */

#define SV_SOCK_PATH "/tmp/us_xfr"    /* Well-known server path */