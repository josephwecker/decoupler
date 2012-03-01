#include "gx.h"
#include "dio.h"
#include "dio_private.h"

int dio_open(char *dcpl_fname, DIOContext *ctx) {
    ctx->canon_fname = dcpl_fname;

    return 0;
}

int dio_add_autowriter(DIOContext *ctx, int in_fd) {
    return(open(ctx->canon_fname, O_RDONLY));
}

int dio_add_autoreader(DIOContext *ctx, int out_fd) {
    return(open(ctx->canon_fname, O_RDONLY));
}


