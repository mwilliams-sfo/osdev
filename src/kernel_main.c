
#include <stddef.h>
#include "heap.h"
#include "intr.h"
#include "term.h"

void kernel_main(void) {
	char * s = NULL;

	intr_init();
	term_init();
	print("Starting kernel\n");

	s = heap_alloc(6);
	if (!s) {
		print("Allocation failed\n");
		goto done;
	}

done:
	if (s) heap_free(s);
}
