
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#include <dio.h>
#include "dio_private.h"
#include <gx.h>

//#ifndef _BSD_SOURCE
//#define _BSD_SOURCE  // for vfork
//#endif

int dio_add_autowriter(char *dcpl_fname, int in_fd) {
    //return(open(dcpl_fname, O_RDONLY));
}

int dio_add_autoreader(char *dcpl_fname, int out_fd) {
    //return(open(dcpl_fname, O_RDONLY));
}


static int get_fnames(char *pathname, DecouplerState *ds) {
    int r1=0, r2=0;
    char *base = strrchr(pathname, '/');
    if(!base) {
        r1 = snprintf(ds->stat_fname, DIO_MAX_PATH_SIZE-1, ".%s.dstat", pathname);
        r2 = snprintf(ds->mgr_addr.sun_path, sizeof(ds->mgr_addr.sun_path)-1, ".%s.mgr",pathname);
    } else {
        *base= '\0'; // null-terminate the directory
        base += 1;
        r1 = snprintf(ds->stat_fname, DIO_MAX_PATH_SIZE-1, "%s/.%s.dstat", pathname, base);
        r2 = snprintf(ds->mgr_addr.sun_path, sizeof(ds->mgr_addr.sun_path)-1, "%s/.%s.mgr", pathname, base);
    }
    return MIN(r1,r2);
}

static int open_read_or_rw(const char *fname, int *orw) {
    int res;
    int tries=0;
    *orw = 0;
    res = open(fname, O_RDONLY);
    while(tries < 100) {
        if(res >= 0) {
            return res;
        } else if(errno == EINTR || errno == EAGAIN) {
            usleep(10);
        } else if(!(*orw) && errno == ENOENT) {
            res = open(fname, O_RDWR|O_EXCL|O_CREAT|O_TRUNC, 00400|00200|00040);
            *orw=1;
        } else if(errno == EEXIST) {
            // Tried read-only, it wasn't there. Then tried creating it and
            // someone else already had- so opening it read-only again. Avoids
            // race conditions - sets only one owner.
            res = open(fname, O_RDONLY);
            *orw=0;
        } else {
            return -1;
        }
        tries ++;
    }
}

int dio_refresh_state(DecouplerState *ds) {
    ssize_t bytes_read=0;

    ds->canonical_exists = (access(ds->canonical_fname, F_OK) == 0);

    bytes_read = pread(ds->stat_fd, ds, sizeof(ds->dss), 0);
    if(bytes_read == -1) return -1;
    if(bytes_read < sizeof(ds->dss)) {
        ds->dss.mgr_pid = NULL;
        ds->dss.state = DIO_UNDETERMINED;
    }
}

int dio_stat(char *pathname, DecouplerState *ds) {
    if(!pathname || !ds) return -1;

    strncpy(ds->canonical_fname, pathname, DIO_MAX_PATH_SIZE-1);

    // Set up manager handle
    ds->mgr_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if(ds->mgr_fd == -1) return -1;
    memset(&(ds->mgr_addr), 0, sizeof(struct sockaddr_un));
    ds->mgr_addr.sun_family = AF_UNIX;
    
    // Get filenames for stat file and unix socket
    if(get_fnames(pathname, ds) < 0) return -1;

    if( (ds->stat_fd = open_read_or_rw(ds->stat_fname, &(ds->stat_is_rw))) < 0) return -1;

    dio_refresh_state(ds);

    return 0;
}


/* Only correct instantaneously- as you try to become a manager you have to
 * make sure another process isn't ahead of you at all times.
 */
int dio_manager_status(DecouplerState *ds) {
    // Is the stat file we opened read-write? If so, we're the future manager.
    if(ds->stat_is_rw) return DIO_MGR_NONE;

    // Is a PID not specified in the stat?
    if(!ds->dss.mgr_pid) return DIO_MGR_PARTIAL;

    // PID specified but set to DORMANT or FINAL

    // Manager comm not set up

    // Manager comm not accepting connections

    // GTG
    return DIO_MGR_ACCEPTING;
}









/*

static int ensure_running_decoupler(const char *pathname);
static int is_decoupler_running(const char *pathname);
static int launch_decoupler(const char *pathname);
*/

/* Expecting
 * Active
 * Flushing
 * Dormant    | decoupler not running
 * Final      | decoupler not running
 */


/* returns fd that should (almost?) never block for writing
 * or -1 on error with errno set
 */
/*
int dio_open_for_appending(const char *pathname) {
    int res=0;
    if((res=ensure_running_decoupler(pathname)) != 0) {
        // TODO: set errno
        return res;
    }

    

    
    return res;
}*/


/* returns fd that should (almost?) never block for reading
 * or -1 on error with errno set.
 */
/*
int dio_open_for_reading(const char *pathname, size_t offset) {

    return -1;
}


int dio_state(const char *pathname) {

    return -1;
}


static int ensure_running_decoupler(const char *pathname) {
    int res = is_decoupler_running(pathname);
    if(!res) return launch_decoupler(pathname);
    return res;
}

static int is_decoupler_running(const char *pathname) {

    return 0;
}

static int launch_decoupler(const char *pathname) { // TODO: parameters like exit actions
    // 1. Test directory and/or file access permissions
    // 2. Make sure that the file wasn't already finalized if it exists (check status)

    return -1;
}

char *dio_fifo_name(const char *pathname) {

    return NULL;
}

char *dio_statusfile_name(const char *pathname) {

    return NULL;
}

//char *decoupler_

*/
