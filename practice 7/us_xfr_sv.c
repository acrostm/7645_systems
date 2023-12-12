#include "us_xfr.h"

#define BACKLOG 5

int
main(int argc, char *argv[])
{
    struct sockaddr_un svaddr;
    int sfd, cfd;
    ssize_t numRead;
    char buf[BUF_SIZE];

    sfd = socket(AF_UNIX, SOCK_STREAM, 0); /* Create server socket */
    if (sfd == -1)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    /* Construct server socket address, bind socket to it, and make this a listening socket */

    if (remove(SV_SOCK_PATH) == -1 && errno != ENOENT)
    {
        printf("remove-%s", SV_SOCK_PATH);
        exit(EXIT_FAILURE);
    }

    memset(&svaddr, 0, sizeof(struct sockaddr_un));
    svaddr.sun_family = AF_UNIX;
    strncpy(svaddr.sun_path, SV_SOCK_PATH, sizeof(svaddr.sun_path) - 1);

    if (bind(sfd, (struct sockaddr *)&svaddr, sizeof(struct sockaddr_un)) == -1)
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    if (listen(sfd, BACKLOG) == -1)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    for (;;) { /* Handle client connections iteratively */
        /* Accept a connection. The connection is returned on a new
           socket, 'cfd'; the listening socket ('sfd') remains open
           and can be used to accept further connections. */

        cfd = accept(sfd, NULL, NULL);
        if (cfd == -1)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        /* Transfer data from connected socket to stdout until EOF */
        while ((numRead = read(cfd, buf, BUF_SIZE)) > 0)
        {
            if (write(STDOUT_FILENO, buf, numRead) != numRead)
            {
                perror("partial/failed write");
                exit(EXIT_FAILURE);
            }
        }

        if (numRead == -1)
        {
            perror("read");
            exit(EXIT_FAILURE);
        }

        if (close(cfd) == -1)
        {
            perror("close");
            exit(EXIT_FAILURE);
        }
    }
}
