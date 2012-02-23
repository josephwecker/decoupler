/* Exposed API
 *
 */
#ifndef DIO_H
#define DIO_H

#include <sys/types.h>
#include <unistd.h>
#include <stddef.h>
#include <stdint.h>

#define DECOUPLE_UNDETERMINED 1
#define DECOUPLE_EXPECTING    2|4
#define DECOUPLE_ACTIVE       4
#define DECOUPLE_FLUSHING     8|4
#define DECOUPLE_DORMANT      16
#define DECOUPLE_FINAL        32

int dio_open_for_appending(const char *pathname);
int dio_open_for_reading(const char *pathname, size_t offset);
int dio_state(const char *pathname);
char *dio_fifo_name(const char *pathname);
char *dio_statusfile_name(const char *pathname);

#endif
