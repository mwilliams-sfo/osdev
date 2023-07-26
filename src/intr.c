
#include <stdint.h>
#include "idt.h"
#include "io.h"
#include "term.h"

#include "intr.h"

void (*intr_handler_table[256])();

#define PORT_PIC1_BASE 0x20
#define PORT_PIC2_BASE 0xa0

#define I8259_PIC_ADDR_COMMAND 0
#define I8259_PIC_ADDR_DATA 1
#define I8259_PIC_CMD_INIT 0x10
#define I8259_PIC_CMD_EOI 0x20
#define I8259_PIC_ICW1_IC4 1
#define I8259_PIC_ICW1_SNGL 2
#define I8259_PIC_ICW4_8086 1

static void intr_handler_pic1_ignore(int intnum) {
	outb(PORT_PIC1_BASE + I8259_PIC_ADDR_COMMAND, I8259_PIC_CMD_EOI);
}

static void intr_handler_kbd(int intnum) {
	print("\rKey pressed\n");
	outb(PORT_PIC1_BASE + I8259_PIC_ADDR_COMMAND, I8259_PIC_CMD_EOI);
}

static void intr_pic_init(uint16_t port_base, const uint8_t * icw) {
	uint8_t mask = inpb(port_base + I8259_PIC_ADDR_DATA);
	outb(port_base + I8259_PIC_ADDR_COMMAND, I8259_PIC_CMD_INIT | (icw[0] & 0xf));
	outb(port_base + I8259_PIC_ADDR_DATA, icw[1]);
	if ((icw[0] & I8259_PIC_ICW1_SNGL) == 0) {
		outb(port_base + I8259_PIC_ADDR_DATA, icw[2]);
	}
	if (icw[0] & I8259_PIC_ICW1_IC4) {
		outb(port_base + I8259_PIC_ADDR_DATA, icw[3]);
	}
	outb(port_base + I8259_PIC_ADDR_DATA, mask);
}

void intr_init(void) {
	idt_init();

	for (int i = 0; i < 8; i++) {
		intr_handler_table[0x20 + i] = intr_handler_pic1_ignore;
	}
	intr_handler_table[0x21] = intr_handler_kbd;
	outb(PORT_PIC1_BASE + I8259_PIC_ADDR_DATA, 0);
	outb(PORT_PIC2_BASE + I8259_PIC_ADDR_DATA, 0xff);
	uint8_t icw[] = {
		I8259_PIC_ICW1_SNGL | I8259_PIC_ICW1_IC4,
		0x20,
		I8259_PIC_ICW4_8086
	};
	intr_pic_init(PORT_PIC1_BASE, icw);

	intr_enable();
}
