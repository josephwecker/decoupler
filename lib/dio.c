
#include "dio.h"

#ifndef _BSD_SOURCE
#define _BSD_SOURCE  // for vfork
#endif


static int ensure_running_decoupler(const char *pathname);
static int is_decoupler_running(const char *pathname);
static int launch_decoupler(const char *pathname);


/* Expecting
 * Active
 * Flushing
 * Dormant    | decoupler not running
 * Final      | decoupler not running
 */


/* returns fd that should (almost?) never block for writing
 * or -1 on error with errno set
 */
int dio_open_for_appending(const char *pathname) {
    int res=0;
    if((res=ensure_running_decoupler(pathname)) != 0) {
        // TODO: set errno
        return res;
    }

    

    
    return res;
}


/* returns fd that should (almost?) never block for reading
 * or -1 on error with errno set.
 */
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


