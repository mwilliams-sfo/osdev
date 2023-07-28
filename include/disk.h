#ifndef OS_DISK_H
#define OS_DISK_H

#include <stddef.h>

#define DISK_SECTOR_SIZE 512

struct disk_stream {
	size_t pos;
};

void disk_stream_seek(struct disk_stream * stream, size_t pos);
size_t disk_stream_read(struct disk_stream * stream, void * buf, size_t len);

#endif
