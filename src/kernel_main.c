
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

	struct path * path = path_parse("/one/two/three");
	if (!path) {
		print("Path parsing failed\n");
		return;
	}
	for (const struct path_part * part = path->root; part; part = part->next) {
		print("Path segment: ");
		print(part->name);
		print("\n");
	}
	path_free(path);
}
