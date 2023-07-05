
#include <stdint.h>
#include "idt.h"
#include "io.h"
#include "term.h"

#include "intr.h"

void (*intr_handler_table[256])();

#define PORT_PIC1_BASE 0x20

#define I8259_PIC_CMD_INIT 0x10
#define I8259_PIC_CMD_EOI 0x20
#define I8259_PIC_ICW1_IC4 1
#define I8259_PIC_ICW1_SNGL 2
#define I8259_PIC_ICW4_8086 1

static void intr_handler_pic1_ignore(int intnum) {
	outb(PORT_PIC1_BASE, I8259_PIC_CMD_EOI);
}

static void intr_handler_kbd(int intnum) {
	print("\rKey pressed\n");
	outb(PORT_PIC1_BASE, I8259_PIC_CMD_EOI);
}

static void intr_pic_init(uint16_t port_base, const uint8_t * icw) {
	outb(port_base, I8259_PIC_CMD_INIT | (icw[0] & 0xf));
	outb(port_base + 1, icw[1]);
	if ((icw[0] & I8259_PIC_ICW1_SNGL) == 0) {
		outb(port_base + 1, icw[2]);
	}
	if (icw[0] & I8259_PIC_ICW1_IC4) {
		outb(port_base + 1, icw[3]);
	}
}

void intr_init(void) {
	idt_init();

	for (int i = 0; i < 7; i++) {
		intr_handler_table[0x20 + i] = intr_handler_pic1_ignore;
	}
	intr_handler_table[0x21] = intr_handler_kbd;
	uint8_t icw[] = {
		I8259_PIC_ICW1_SNGL | I8259_PIC_ICW1_IC4,
		0x20,
		I8259_PIC_ICW4_8086
	};
	intr_pic_init(PORT_PIC1_BASE, icw);

	intr_enable();
}
