
#include "idt.h"

void kernel_main(void);

void kernel_start(void)
{
	idt_init();
	kernel_main();
}
