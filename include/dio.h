#ifndef DIO_H
#define DIO_H

int dio_add_autowriter(char *dcpl_fname, int in_fd);
int dio_add_autoreader(char *dcpl_fname, int out_fd);

#endif
