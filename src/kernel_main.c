
#include <stddef.h>
#include "heap.h"
#include "intr.h"
#include "paging.h"
#include "term.h"

void kernel_main(void) {
	term_init();
	print("Starting kernel\n");

	struct paging_space * space = paging_init();
	if (!space) {
		print("Paging initialization failed");
		return;
	}
	intr_init();
}
