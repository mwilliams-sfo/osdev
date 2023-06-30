
#include "intr.h"
#include "term.h"

void kernel_main() {
	intr_init();
	term_init();
	print("Kernel started\n");
}
