
#include "intr.h"

void kernel_main(void);

void kernel_start(void)
{
	intr_init();
	kernel_main();
}
