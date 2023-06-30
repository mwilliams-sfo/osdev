
ENTRY(kernel_main)
SECTIONS {
	. = 1M;
	.text : {
		*(.text)
		*(.text.*)
		. = ALIGN(4096);
	}
	.rodata : {
		*(.rodata)
		*(.rodata.*)
		. = ALIGN(4096);
	}
	.data : {
		*(.data)
		*(.data.*)
		. = ALIGN(4096);
	}
	.bss : {
		*(COMMON)
		*(.bss)
		*(.bss.*)
	}
}
