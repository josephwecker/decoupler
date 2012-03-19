/* libdio -- Decoupled Input/Output
 *
 * dio_open:
 *   Create a context before adding writer / readers.
 *
 * dio_add_autowriter:
 *   Allow dio to take over the fd and, as data becomes available, write it to
 *   the sink. In the current implementation there can only be one writer.
 *
 * dio_add_autoreader:
 *   Allow dio to take over the fd and write the output to it, starting at the
 *   given offset. A reader can be attached before a writer or before the
 *   decoupler even exists, in the middle of a flow, or after it has all been
 *   input.
 *
 * dio_add_writer:
 *   Sets up a socket-pair and returns an fd that you can write to. In the
 *   current implementation there can only be one writer.
 *
 * dio_add_reader:
 *   Sets up a socket-pair with the dio manager and returns the fd that you can
 *   then read data from as it becomes available.
 *
 *
 * An offset of DIO_OFFSET_LIVE is a special dynamic offset indicator that
 * means wherever the write head is currently.
 *
 */



#ifndef DIO_H
#define DIO_H

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

typedef struct DIOContext {
    char *                    canon_fname;
    char                      stat_fname[DIO_MAX_PATH_SIZE];

    int                       canon_fd;
    int                       stat_fd;
    int                       stat_mode;
} DIOContext;

extern int dio_open(char *dcpl_fname, DIOContext *ctx);
extern int dio_add_autowriter(DIOContext *ctx, int in_fd);
extern int dio_add_autoreader(DIOContext *ctx, int out_fd); // TODO: add offset

extern int dio_current_state(DIOContext *ctx);
extern int dio_managed(DIOContext *ctx);

#endif
