#include <sys/un.h>
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#define SV_SOCK_PATH "/tmp/testing-lock9"
int main(int argc, char *argv[]) {
    struct sockaddr_un addr;
    int sfd;

    sfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sfd == -1) exit(errno); /* Construct server address, and make the connection */
    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX; strncpy(addr.sun_path, SV_SOCK_PATH, sizeof(addr.sun_path) - 1);
    if (connect(sfd, (struct sockaddr *) &addr, sizeof(struct sockaddr_un)) == -1) {
        fprintf(stderr, "Couldn't connect\n");
        exit(errno);
    }
    /* Copy stdin to socket */
    //while ((numRead = read(STDIN_FILENO, buf, BUF_SIZE)) > 0) if (write(sfd, buf, numRead) != numRead)
    //    fatal("partial/failed write");
    //if (numRead == -1) errExit("read");
    exit(EXIT_SUCCESS);	/* Closes our socket; server sees EOF */
}
