
#include "io.h"

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

int disk_read_sectors(int lba, int count, void * buf) {
	if (!disk_await_status(ATA_STATUS_BSY, 0)) return 0;

	outb(PORT_ATA_HD0_BASE + ATA_ADDR_DEVICE, 0xe0 | (lba >> 24 & 0xf));
	outb(PORT_ATA_HD0_BASE + ATA_ADDR_SECTOR_COUNT, count & 0xff);
	outb(PORT_ATA_HD0_BASE + ATA_ADDR_LBA_LOW, lba);
	outb(PORT_ATA_HD0_BASE + ATA_ADDR_LBA_MID, lba >> 8);
	outb(PORT_ATA_HD0_BASE + ATA_ADDR_LBA_HIGH, lba >> 16);
	outb(PORT_ATA_HD0_BASE + ATA_ADDR_COMMAND, ATA_CMD_READ_SECTOR);
	// Wait for drive selection.
	for (int i = 0; i < 14; i++) {
		inpb(PORT_ATA_HD0_BASE + ATA_ADDR_STATUS);
	}

	int i = 0;
	uint16_t * ptr = buf;
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
