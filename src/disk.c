
#include "io.h"

#define PORT_ATA_HD0_BASE 0x1f0

#define ATA_STATUS_ERR 1
#define ATA_STATUS_DRQ 0x10
#define ATA_STATUS_DF 0x20
#define ATA_STATUS_BSY 0x80
#define ATA_CMD_READ_SECTOR 0x20

int disk_read_sectors(int lba, int count, void * buf) {
	int i = 0;

	uint8_t stat;
	do {
		stat = inpb(PORT_ATA_HD0_BASE + 7);
		if (stat & (ATA_STATUS_ERR | ATA_STATUS_DF)) return i;
	} while (stat & ATA_STATUS_BSY);

	outb(PORT_ATA_HD0_BASE + 6, 0xe0 | (lba >> 24 & 0xf));
	outb(PORT_ATA_HD0_BASE + 2, count & 0xff);
	outb(PORT_ATA_HD0_BASE + 3, lba);
	outb(PORT_ATA_HD0_BASE + 4, lba >> 8);
	outb(PORT_ATA_HD0_BASE + 5, lba >> 16);
	outb(PORT_ATA_HD0_BASE + 7, ATA_CMD_READ_SECTOR);

	uint16_t * ptr = buf;
	for (i = 0; i < count; i++) {
		do {
			stat = inpb(PORT_ATA_HD0_BASE + 7);
			if (stat & (ATA_STATUS_ERR | ATA_STATUS_DF)) return i;
		} while (!(stat & ATA_STATUS_DRQ));

		for (int j = 0; j < 256; j++) {
			*ptr++ = inpw(PORT_ATA_HD0_BASE);
		}
	}
	return i;
}
