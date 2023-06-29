
#include "config.h"
#include "kernel.h"
#include "idt.h"

static struct idt_entry idt[512] = { 0 };

static const struct idt_desc idtr = { sizeof(idt) - 1, (uint32_t) &idt };

void idt_load(const struct idt_desc * desc);

static void idt_set_critical(int index, uint32_t offset)
{
	struct idt_entry * entry = &idt[index];
	entry->present = 0;
	entry->offset_low = offset;
	entry->offset_high = offset >> 16;
	entry->selector = KERNEL_CODE_SELECTOR;
	entry->gate_type = 14;
	entry->privilege = 3;
	entry->present = 1;
}

static void interrupt0(void)
{
	print("Division by zero");
	while (1);
}

void idt_init(void)
{
	idt_set_critical(0, (uint32_t) interrupt0);
	idt_load(&idtr);
}
