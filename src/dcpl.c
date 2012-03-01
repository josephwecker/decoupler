#include <gx.h>
#include <dio.h>
#include "config.h"

int main(int argc, char **argv) {
    char *decouple_fname;
    int stdin_is_pipe = 0;
    int stdout_is_pipe = 0;
    struct stat stat_buffer;

    if(argc > 1 && argv[1][0]=='-') { // heh, since we don't have commandline options yet
        gx_log_error("Option not recognized");
        gx_log_info("Decoupler Usage:  %s [canonical-file]\n"
                "Launches a decoupler to manage the intermediate file specified.\n"
                "Pipe into its stdin to write to the file or use libdio.\n"
                "Use this processes stdout or libdio for the decoupled output.", "dcpl");
        exit(EXIT_FAILURE);
    }

    __( fstat(STDIN_FILENO, &stat_buffer),   E_FATAL;E_EXIT );
    stdin_is_pipe = S_ISFIFO(stat_buffer.st_mode);

    __( fstat(STDOUT_FILENO,&stat_buffer),   E_FATAL;E_EXIT );
    stdout_is_pipe = S_ISFIFO(stat_buffer.st_mode);

    if(gx_unlikely(argc < 2 && !stdout_is_pipe)) {
        gx_log_fatal("You must specify the decoupling (intermediate) file, "
                "or at least pipe the output to something and I'll use a temp "
                "file as the intermediate.");
        exit(EXIT_FAILURE);
    }

    if(argc < 2) {
        /* TODO: populate decouple_fname with tempfile name */
    } else {
        decouple_fname = argv[1];
    }

    if(stdin_is_pipe)
        __( dio_add_autowriter(decouple_fname, STDIN_FILENO),  E_ERROR;E_EXIT );
    
    if(stdout_is_pipe)
        __( dio_add_autoreader(decouple_fname, STDOUT_FILENO), E_ERROR;E_EXIT );


    exit(EXIT_SUCCESS);
    return 0;
}
