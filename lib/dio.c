#include "config.h"
#ifdef HAVE_SPLICE
#define _GNU_SOURCE
#include <fcntl.h>
#else
#include <sys/types.h>
//#include <sys/uio.h>
#include <unistd.h>
#endif
#include "gx.h"
#include "dio.h"
#include "dio_private.h"

int dio_open(char *dcpl_fname, DIOContext *ctx) {
    memset(ctx, 0, sizeof(DIOContext));
    ctx->canon_fname = dcpl_fname;

    return 0;
}

int dio_add_autowriter(DIOContext *ctx, int in_fd) {
    return(open(ctx->canon_fname, O_RDONLY)); // TMP placeholder. remove me
}

int dio_add_autoreader(DIOContext *ctx, int out_fd) {
    int currstate = dio_current_state(ctx);
    if(!dio_managed(ctx) && !(currstate & (DIO_DORMANT|DIO_EXPECTING))) {
        /* Edge case where we can simply sendfile the existing file. */
        return send_existing_file(ctx->canon_fname, out_fd);
    }
    return -1;
}

int dio_current_state(DIOContext *ctx)  {
    return DIO_UNDETERMINED;
}

int dio_managed(DIOContext *ctx) {
    return DIO_MGR_NONE;
}


/* As of now, only used in the edge case of someone reading a finalized file.
 */
static int send_existing_file(char *fname, int out_fd) {
    // Daemonizes and does the transfer of a simple file to the out_fd
    // oldschool style (blocking)
    int canon_fd = open(fname, O_RDONLY|O_NONBLOCK);
    int is_parent;
    if(canon_fd == -1) return -1;
    X( out_fd = dup(out_fd), E_WARN);
    X( is_parent = small_daemon(), E_ERROR;E_RAISE);
    if(is_parent) return 0;

    ssize_t transfered;
#ifdef HAVE_SPLICE
    do {
        X( transfered = splice(canon_fd, NULL, out_fd, NULL, 1<<16, SPLICE_F_MOVE), E_FATAL;E_EXIT);
    } while(transfered > 0);
#else
    ssize_t bytes_in;
    char buf[4096];
    do {
        X( bytes_in = read(canon_fd, buf, 4096),      E_FATAL;E_EXIT);
        X( transfered = write(out_fd, buf, bytes_in), E_FATAL;E_EXIT);
    } while(bytes_in > 0);
#endif

    exit(EXIT_SUCCESS);
}

static int small_daemon() {
    // TODO: it should be possible to make this much lighter with clone.
    pid_t pid, sid;
    pid = fork();
    if(pid != 0) return pid;
    // Child
    umask(0);
    X( sid = setsid(), E_WARN );
    X( chdir("/"),     E_WARN );  // Don't bind up the working dir
#if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ > 5)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-result"
#endif
    Xn( freopen("/dev/null", "r", stdin), E_WARN);
    Xn( freopen("/dev/null", "w", stdout), E_WARN);
#if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ > 5)
#pragma GCC diagnostic pop
#endif
    return 0;
}

