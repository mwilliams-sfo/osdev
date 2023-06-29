
#include "idt.h"
#include "io.h"
#include "kernel.h"

#include "intr.h"

void (*intr_handler_table[256])() = { 0 };

static void intr_handler_pic1_ignore(int intnum) {
	outb(0x20, 0x20);
}

static void intr_handler_kbd(int intnum) {
	print("Key pressed\n");
	outb(0x20, 0x20);
}

void intr_init(void) {
	idt_init();

	for (int i = 0; i < 7; i++) {
		intr_handler_table[0x20 + i] = intr_handler_pic1_ignore;
	}
	intr_handler_table[0x21] = intr_handler_kbd;
	outb(0x20, 0x13);
	outb(0x21, 0x20);
	outb(0x21, 0x1);

	intr_enable();
}
