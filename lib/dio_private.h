/* Exposed API
 *
 */
#ifndef DIO_PRIVATE_H
#define DIO_PRIVATE_H

#include <sys/types.h>
#include <unistd.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <sys/stat.h>

#define DIO_MAX_PATH_SIZE     1024


#define DIO_UNDETERMINED      1
#define DIO_EXPECTING         2|4
#define DIO_ACTIVE            4
#define DIO_FLUSHING          8|4
#define DIO_DORMANT           16
#define DIO_FINAL             32


#define DIO_MGR_NONE          0
#define DIO_MGR_ACCEPTING     1
#define DIO_MGR_PARTIAL       -1

typedef struct DecouplerSavedState {
    pid_t                     mgr_pid;
    int                       state;     
}   DecouplerSavedState;

typedef struct DecouplerState {
    DecouplerSavedState       dss;

    char                      stat_fname[DIO_MAX_PATH_SIZE];
    int                       stat_fd;
    int                       stat_is_rw;

    struct sockaddr_un        mgr_addr;
    int                       mgr_fd;

    char                      canonical_fname[DIO_MAX_PATH_SIZE];
    int                       canonical_exists;
    //struct stat               canonical_info;

}   DecouplerState;


/* TODO: clean up interface a bit- make some things opaque. need to finish the
 * manager implementation first though.
 */
extern int dio_stat(char *pathname, DecouplerState *ds);
extern int dio_manager_status(DecouplerState *ds);

/*int dio_open_for_appending(const char *pathname);
int dio_open_for_reading(const char *pathname, size_t offset);
int dio_state(const char *pathname);
char *dio_fifo_name(const char *pathname);
char *dio_statusfile_name(const char *pathname);
*/

/*
 * decoupler-file  | path | exists? |
 * stat-file       | path | exists? | pid | state |
 * comm-channel    | path | exists? | accepting? |
 */


#endif
