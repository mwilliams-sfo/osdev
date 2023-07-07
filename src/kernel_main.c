
#include <stddef.h>
#include "disk.h"
#include "heap.h"
#include "intr.h"
#include "paging.h"
#include "term.h"

void kernel_main(void) {
	int n;

	term_init();
	print("Starting kernel\n");

	n = paging_init();
	if (n < 0) {
		print("Paging initialization failed\n");
		return;
	}
	char buf[512];
	n = disk_read_sectors(0, 1, buf);
	if (n != 1) {
		print("Disk read failed\n");
		return;
	}
	if (buf[510] != (char) 0x55 || buf[511] != (char) 0xaa) {
		while (1);
		print("Boot signature not found\n");
		return;
	}

	intr_init();
}
