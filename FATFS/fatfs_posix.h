#ifndef __FATFS_POSIX_h
#define __FATFS_POSIX_h


#include "ff.h"
#include <stdlib.h>

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

int open(const char *file, int flags, int mode);
int close(int fd);
int read(int fd, void *buf, size_t len);
int write(int fd, const void *buf, size_t len);
long tell(int fd);
long lseek(int fd, long offset, int whence);
long get_file_size(int fd);
FRESULT fatfs_init(void);

#endif
