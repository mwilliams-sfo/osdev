
#include "config.h"

#include "idt.h"

static struct idt_entry idt[256];

void idt_load(const struct idt_desc * desc);

static void idt_set(int index, void (*addr)()) {
	uint32_t offset = (uint32_t) addr;
	idt[index] = (struct idt_entry) {
		.offset_low = offset,
		.offset_high = offset >> 16,
		.selector = KERNEL_CODE_SELECTOR,
		.gate_type = 14,
		.privilege = 3,
		.present = 1
	};
}

void idt_init(void) {
	extern void (*intr_vector_table[])();
	for (int i = 0; i < sizeof idt / sizeof idt[0]; i++) {
		idt_set(i, intr_vector_table[i]);
	}
	idt_load(&(struct idt_desc) {
		.size = sizeof idt - 1,
		.offset = idt
	});
}
