
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
	print("Reading boot sector\n");
	uint16_t * buf = heap_calloc(512);
	n = disk_read_sectors(0, 1, buf);
	if (n != 1) {
		print("Disk read failed\n");
		goto err;
	}
	if (buf[255] != (uint16_t) 0xaa55) {
		print("Boot signature not found\n");
		goto err;
	}

	intr_init();
	print("Kernel initialized\n");

err:
	heap_free(buf);
}
