
#include <stddef.h>
#include "heap.h"
#include "intr.h"
#include "term.h"

void kernel_main() {
	char * s = NULL;

	intr_init();
	term_init();
	print("Kernel started\n");

	s = heap_alloc(6);
	if (!s) {
		print("Allocation failed\n");
		goto done;
	}

done:
	if (s) heap_free(s);
}
