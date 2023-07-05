
#include <stddef.h>
#include "heap.h"
#include "intr.h"
#include "paging.h"
#include "term.h"

void kernel_main(void) {
	int n;
	void * page = NULL;
	int page_mapped = 0;

	term_init();
	print("Starting kernel\n");

	n = paging_init();
	if (n < 0) {
		print("Paging initialization failed\n");
		goto err;
	}

	page = heap_calloc(PAGE_SIZE);
	if (page) {
		n = paging_map_address((void *) 0x1000, page);
		page_mapped = (n == 0);
	}
	if (!page_mapped) {
		print("Page mapping failed\n");
		goto err;
	}
	*(unsigned *) page = 0x1234;
	if (*(unsigned *) 0x1000 != 0x1234) {
		print("Page is not mapped correctly\n");
		goto err;
	}
	print("Memory mapping verified\n");

	intr_init();

err:
	if (page && !page_mapped) heap_free(page);
}
