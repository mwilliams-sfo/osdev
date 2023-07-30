
#include "config.h"

#include "idt.h"

#define IDT_GATE_TYPE_32BIT_INTERRUPT 14

static struct idt_entry idt[256];

static void idt_set(int intnum, void (*addr)()) {
	uint32_t offset = (uint32_t) addr;
	idt[intnum] = (struct idt_entry) {
		.selector = KERNEL_CODE_SELECTOR,
		.offset_high = offset >> 16,
		.offset_low = offset,
		.attributes =
			1 << 15 | // Present
			0 << 13 | // Privilege level
			IDT_GATE_TYPE_32BIT_INTERRUPT << 8
	};
}

static void idt_load(const struct idt_desc * desc) {
	asm("lidtl (%0)" : : "r"(desc));
}

void idt_init(void) {
	extern void intr_vector_base();
	extern int intr_thunk_size;
	for (int i = 0; i < sizeof idt / sizeof idt[0]; i++) {
		void * vector = (char *) intr_vector_base + i * intr_thunk_size;
		idt_set(i, vector);
	}
	idt_load(&(struct idt_desc) {
		.size = sizeof idt - 1,
		.offset = idt
	});
}
