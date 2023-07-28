
#include <stddef.h>
#include <string.h>
#include "heap.h"
#include "io.h"

#include "disk.h"

#define PORT_ATA_HD0_BASE 0x1f0
#define ATA_ADDR_DATA 0
#define ATA_ADDR_SECTOR_COUNT 2
#define ATA_ADDR_LBA_LOW 3
#define ATA_ADDR_LBA_MID 4
#define ATA_ADDR_LBA_HIGH 5
#define ATA_ADDR_DEVICE 6
#define ATA_ADDR_STATUS 7
#define ATA_ADDR_COMMAND 7

#define ATA_STATUS_ERR 1
#define ATA_STATUS_DRQ 0x10
#define ATA_STATUS_DF 0x20
#define ATA_STATUS_BSY 0x80
#define ATA_CMD_READ_SECTOR 0x20

static int disk_await_status(uint8_t mask, uint8_t value) {
	uint8_t stat;
	do {
		stat = inpb(PORT_ATA_HD0_BASE + ATA_ADDR_STATUS);
		if (stat & (ATA_STATUS_ERR | ATA_STATUS_DF)) return 0;
	} while ((stat & mask) != value);
	return 1;
}

static int disk_read_sectors(int lba, int count, void * buf) {
	if (count <= 0) return 0;

	if (!disk_await_status(ATA_STATUS_BSY, 0)) return 0;
	outb(PORT_ATA_HD0_BASE + ATA_ADDR_DEVICE,
		0xa0 | // Legacy bits
		1 << 6 | // LBA mode
		0 << 4 | // Device 0
		(lba >> 24 & 0xf));
	outb(PORT_ATA_HD0_BASE + ATA_ADDR_SECTOR_COUNT, count & 0xff);
	outb(PORT_ATA_HD0_BASE + ATA_ADDR_LBA_LOW, lba);
	outb(PORT_ATA_HD0_BASE + ATA_ADDR_LBA_MID, lba >> 8);
	outb(PORT_ATA_HD0_BASE + ATA_ADDR_LBA_HIGH, lba >> 16);
	outb(PORT_ATA_HD0_BASE + ATA_ADDR_COMMAND, ATA_CMD_READ_SECTOR);
	// Wait for drive selection.
	for (int i = 0; i < 14; i++) {
		inpb(PORT_ATA_HD0_BASE + ATA_ADDR_STATUS);
	}

	uint16_t * ptr = buf;
	int i = 0;
	for (i = 0; i < count; i++) {
		if (!disk_await_status(ATA_STATUS_DRQ, ATA_STATUS_DRQ)) break;
		for (int j = 0; j < 256; j++) {
			*ptr++ = inpw(PORT_ATA_HD0_BASE);
		}

		// Check for an error before moving on.
		uint8_t stat = inpb(PORT_ATA_HD0_BASE + ATA_ADDR_STATUS);
		if (stat & (ATA_STATUS_ERR | ATA_STATUS_DF)) break;
	}
	return i;
}

void disk_stream_seek(struct disk_stream * stream, size_t pos) {
	stream->pos = pos;
}

size_t disk_stream_read(struct disk_stream * stream, void * buf, size_t len) {
	if (!len) return 0;
	void * sector = NULL;

	size_t bytes_read = 0;
	if (stream->pos % DISK_SECTOR_SIZE) {
		// Read leading partial sector.
		if (!sector) {
			sector = heap_alloc(DISK_SECTOR_SIZE);
			if (!sector) goto done;
		}
		int sectors_read = disk_read_sectors(stream->pos / DISK_SECTOR_SIZE, 1, sector);
		if (sectors_read == 0) goto done;
		int copy_off = stream->pos % DISK_SECTOR_SIZE;
		int copy_len = DISK_SECTOR_SIZE - copy_off;
		if (len < copy_len) copy_len = len;
		memcpy(buf, (char *) sector + copy_off, copy_len);
		stream->pos += copy_len;
		bytes_read += copy_len;
		buf = (char *) buf + copy_len;
		len -= copy_len;
	}

	// Read whole sectors.
	int lba = stream->pos / DISK_SECTOR_SIZE;
	int sector_count = len / DISK_SECTOR_SIZE;
	int sectors_read = disk_read_sectors(lba, sector_count, buf);
	stream->pos += sectors_read * DISK_SECTOR_SIZE;
	bytes_read += sectors_read * DISK_SECTOR_SIZE;
	if (sectors_read < sector_count) goto done;
	buf = (char *) buf + sectors_read * DISK_SECTOR_SIZE;
	len -= sectors_read * DISK_SECTOR_SIZE;

	if (len) {
		// Read trailing partial sector.
                if (!sector) {
                        sector = heap_alloc(DISK_SECTOR_SIZE);
                        if (!sector) goto done;
                }
		int sectors_read = disk_read_sectors(
			stream->pos / DISK_SECTOR_SIZE, 1, sector);
		if (sectors_read == 0) goto done;
		memcpy(buf, (char *) sector, len);
		stream->pos += len;
		bytes_read += len;
	}
done:
	heap_free(sector);
	return bytes_read;
}
