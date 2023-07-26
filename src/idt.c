
#include "config.h"

#include "idt.h"

#define IDT_GATE_TYPE_32BIT_INTERRUPT 14

static struct idt_entry idt[256];

void idt_load(const struct idt_desc * desc);

static void idt_set(int index, void (*addr)()) {
	uint32_t offset = (uint32_t) addr;
	idt[index] = (struct idt_entry) {
		.selector = KERNEL_CODE_SELECTOR,
		.offset_high = offset >> 16,
		.offset_low = offset,
		.attributes =
			1 << 15 |
			0 << 13 |
			IDT_GATE_TYPE_32BIT_INTERRUPT << 8
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
