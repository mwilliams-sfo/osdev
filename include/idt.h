#ifndef IDT_H
#define IDT_H

#include <stdint.h>

struct idt_entry {
	uint16_t offset_low;
	uint16_t selector;
	uint8_t: 8;
	uint8_t gate_type: 4;
	uint8_t: 1;
	uint8_t privilege: 2;
	uint8_t present: 1;
	uint16_t offset_high;
} __attribute__((packed));

struct idt_desc {
	uint16_t size;
	struct idt_entry * offset;
} __attribute__((packed));

void idt_init(void);

#endif
