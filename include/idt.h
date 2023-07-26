#ifndef IDT_H
#define IDT_H

#include <stdint.h>

struct idt_entry {
	uint16_t offset_low;
	uint16_t selector;
	uint16_t attributes;
	uint16_t offset_high;
} __attribute__((packed));

struct idt_desc {
	uint16_t size;
	struct idt_entry * offset;
} __attribute__((packed));

void idt_init(void);

#endif
