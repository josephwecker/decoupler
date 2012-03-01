#ifndef DIO_PRIVATE_H
#define DIO_PRIVATE_H

#include <sys/types.h>
#include <unistd.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <sys/stat.h>



typedef struct DIOSavedState {
    pid_t                     mgr_pid;
    int                       state;     
}   DIOSavedState;

typedef struct DIOState {
    DIOSavedState             dss;

    char                      stat_fname[DIO_MAX_PATH_SIZE];
    int                       stat_fd;
    int                       stat_is_rw;

    struct sockaddr_un        mgr_addr;
    int                       mgr_fd;

    char                      canonical_fname[DIO_MAX_PATH_SIZE];
    int                       canonical_exists;
    //struct stat               canonical_info;

}   DIOState;


extern int dio_stat(char *pathname, DIOState *ds);
extern int dio_manager_status(DIOState *ds);

/*
 * decoupler-file  | path | exists? |
 * stat-file       | path | exists? | pid | state |
 * comm-channel    | path | exists? | accepting? |
 */


#endif
