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

} DIOContext;

int dio_open(char *dcpl_fname, DIOContext *ctx);
int dio_add_autowriter(DIOContext *ctx, int in_fd);
int dio_add_autoreader(DIOContext *ctx, int out_fd);

#endif
