
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

#define INTNUM_PIC1_BASE 0x20
#define INTNUM_PIC2_BASE 0x28

static void intr_pic_end(int intnum) {
	if (intnum >= INTNUM_PIC2_BASE) {
		outb(PORT_PIC2_BASE + I8259_PIC_ADDR_COMMAND, I8259_PIC_CMD_EOI);
	}
	outb(PORT_PIC1_BASE + I8259_PIC_ADDR_COMMAND, I8259_PIC_CMD_EOI);
}

static void intr_handler_kbd(int intnum) {
	print("\rKey pressed\n");
	intr_pic_end(intnum);
}

static void intr_pic_init(const uint8_t * pic1_icw, const uint8_t * pic2_icw) {
	uint8_t mask1 = inpb(PORT_PIC1_BASE + I8259_PIC_ADDR_DATA);
	uint8_t mask2 = inpb(PORT_PIC2_BASE + I8259_PIC_ADDR_DATA);

	outb(PORT_PIC1_BASE + I8259_PIC_ADDR_COMMAND, I8259_PIC_CMD_INIT | (pic1_icw[0] & 0xf));
	outb(PORT_PIC2_BASE + I8259_PIC_ADDR_COMMAND, I8259_PIC_CMD_INIT | (pic2_icw[0] & 0xf));
	outb(PORT_PIC1_BASE + I8259_PIC_ADDR_DATA, pic1_icw[1]);
	outb(PORT_PIC2_BASE + I8259_PIC_ADDR_DATA, pic2_icw[1]);
	if (!(pic1_icw[0] & I8259_PIC_ICW1_SNGL)) {
		outb(PORT_PIC1_BASE + I8259_PIC_ADDR_DATA, pic1_icw[2]);
	}
	if (!(pic2_icw[0] & I8259_PIC_ICW1_SNGL)) {
		outb(PORT_PIC2_BASE + I8259_PIC_ADDR_DATA, pic2_icw[2]);
	}
	if (pic1_icw[0] & I8259_PIC_ICW1_IC4) {
		outb(PORT_PIC1_BASE + I8259_PIC_ADDR_DATA, pic1_icw[3]);
	}
	if (pic2_icw[0] & I8259_PIC_ICW1_IC4) {
		outb(PORT_PIC2_BASE + I8259_PIC_ADDR_DATA, pic2_icw[3]);
	}

	outb(PORT_PIC1_BASE + I8259_PIC_ADDR_DATA, mask1);
	outb(PORT_PIC2_BASE + I8259_PIC_ADDR_DATA, mask2);
}

static void intr_enable(void) {
	asm("sti");
}

void intr_init(void) {
	idt_init();

	for (int i = 0; i < 16; i++) {
		intr_handler_table[0x20 + i] = intr_pic_end;
	}
	intr_handler_table[0x21] = intr_handler_kbd;
	uint8_t pic1_icw[] = { I8259_PIC_ICW1_IC4, INTNUM_PIC1_BASE, 2, I8259_PIC_ICW4_8086 };
	uint8_t pic2_icw[] = { I8259_PIC_ICW1_IC4, INTNUM_PIC2_BASE, 4, I8259_PIC_ICW4_8086 };
	intr_pic_init(pic1_icw, pic2_icw);

	intr_enable();
}
