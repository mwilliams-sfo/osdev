
#include <stddef.h>
#include "disk.h"
#include "fs.h"
#include "heap.h"
#include "intr.h"
#include "paging.h"
#include "term.h"

static void kernel_main(void);

void _start() {
	kernel_main();
}

static void kernel_main(void) {
	term_init();
	print("Starting kernel\n");

	if (paging_init() != 0) {
		print("Paging initialization failed\n");
		return;
	}

	intr_init();
	print("Kernel initialized\n");

	struct disk_stream stream = { 0 };
	disk_stream_seek(&stream, 510);
	uint16_t magic;
	int n = disk_stream_read(&stream, &magic, sizeof magic);
	if (n < sizeof magic) {
		print("Failed to read boot sector\n");
		return;
	}
	if (magic != 0xaa55) {
		print("Boot sector identifier has wrong value\n");
	}
}
