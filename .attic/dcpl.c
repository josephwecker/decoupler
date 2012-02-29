#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#include <sys/un.h>
#include <sys/socket.h>
#include <ctype.h>

#include <dio.h>
#include <gx.h>
#include "config.h"



int stdin_is_pipe = 0;
int stdout_is_pipe = 0;
DecouplerState dstate;

int file_to_pipe(int file_fd, int pipe_fd) {
#ifdef USE_SPLICE
    return splice(file_fd, 0, pipe_fd, NULL, 4096, SPLICE_F_MOVE); // TODO: obviously fix size
#else
    return -1;
#endif
}


static void daemonize() {
    // TODO: correct error messages etc.
    pid_t pid, sid;
    if(getppid() == 1) return; // Already a daemon
    pid = fork();
    if(pid<0) exit(EXIT_FAILURE);
    if(pid>0) exit(EXIT_SUCCESS); // Parent exiting
    umask(0);
    sid = setsid();
    if(sid<0) exit(EXIT_FAILURE);
    if((chdir("/")) < 0) exit(EXIT_FAILURE); // Don't bind up working dir

    if(!stdin_is_pipe)  freopen("/dev/null", "r", stdin);
    if(!stdout_is_pipe) freopen("/dev/null", "w", stdout);
    freopen("/dev/null", "w", stderr);
}


/*static int handoff() {

    return 0;
}*/

int main(int argc, char *argv[]) {
    // - If stdin has anything piping to it, treat it as a writing feed
    // - If stdout is not a terminal, treat it as a reading feed
    // - If another decoupler is already running on the file, turn over fds to it.
    // - Otherwise daemonize and do the thing.
    struct stat stat_buffer;

    if(argc > 1 && argv[1][0]=='-') { // heh, since we don't have commandline options yet
        fprintf(stderr, "Decoupler\nUsage:  %s [canonical-file]\n"
                "Launches a decoupler to manage the intermediate file specified. "
                "Pipe into its stdin to write to the file or use libdio. "
                "Use this processes stdout or libdio for the decoupled output.\n", "dcpl");
        exit(EXIT_FAILURE);
    }

    if(gx_unlikely((fstat(STDIN_FILENO,  &stat_buffer)) < 0)) exit(errno);
    stdin_is_pipe = S_ISFIFO(stat_buffer.st_mode);
    if(gx_unlikely((fstat(STDOUT_FILENO, &stat_buffer)) < 0)) exit(errno);
    stdout_is_pipe = S_ISFIFO(stat_buffer.st_mode);
    if(gx_unlikely(argc < 2 && !stdout_is_pipe)) {
        fprintf(stderr, "ERROR: You must specify the decoupling (intermediate) file, "
                "or at least pipe the output to something and I'll use a temp file as "
                "the intermediate.\n");
        exit(EXIT_FAILURE);
    }

    int r = dio_stat(argv[1], &dstate);
    if(r < 0) {
        fprintf(stderr, "Couldn't stat the decoupled file.\n");
        exit(errno);
    }

    int mgr = dio_manager_status(&dstate);
    // First- check for quick edgecase where it's a normal finalized file
    //if(stdout_is_pipe && cf_exists && !ss_exists && (curr_status() != DIO_DORMANT)) {
    if(stdout_is_pipe                       &&
            mgr == DIO_MGR_NONE             &&
            dstate.dss.state != DIO_DORMANT &&
            dstate.canonical_exists) {
        int fd_in = open(argv[1], O_RDONLY | O_NONBLOCK);
        file_to_pipe(fd_in, STDOUT_FILENO);
        exit(EXIT_SUCCESS);
    }




    if(dio_manager_status(&dstate) == DIO_MGR_NONE) {

    }

    //fprintf(stderr, "%s\n%s\n",dstate.stat_fname, dstate.mgr_addr.sun_path);

    // TODO: open and check for / acquire lock on status file if not
    // special-case 0 below.
    /*struct sockaddr_un ctrl_addr;
    int ctrl_sfd;
    ctrl_sfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if(ctrl_sfd == -1) exit(errno);
    memset(&ctrl_addr, 0, sizeof(struct sockaddr_un));
    ctrl_addr.sun_family = AF_UNIX;
    strncpy(ctrl_addr.sun_path, "/tmp/testing-lock2", sizeof(ctrl_addr.sun_path) - 1);

    if(bind(ctrl_sfd, (struct sockaddr *) &ctrl_addr, sizeof(struct sockaddr_un)) == -1) {
        fprintf(stderr, "COULD NOT BIND.\n");
        exit(errno);
    } else {
        fprintf(stderr, "Bound!\n");
    }*/

    if(stdin_is_pipe) {
        // Wants to write to the decoupling.

        // 1. ss_bindable? no: permissions? fail. INUSE/INVAL? do handoff
        //    negotiation (it might be an 'expecting' decoupler).
        // 2. status-dormant? yes: open cf append-mode
        // 3. status-missing/final? yes: open cf truncate-new-write-mode
        //
        // 5. set status as appropriate
    }

    if(stdout_is_pipe) {
        // Wants to read from the decoupling.
        // 0. special-case: cf-exists, no ss, status!=dormant: sendfile() and exit.
        // 1. ss_bindable? yes: going to be an 'expecting' decoupler.
        // 2. no: attempt to establish connection to ss
        // 3. success: handoff
        // 4. fail: error (I think)
    }


    //sleep(5);
    daemonize(); // TODO: this will move to the appropriate function- only if
                 // this process will become the official dcpl for the
                 // canonical file

    exit(EXIT_SUCCESS);
    return 0;
}
