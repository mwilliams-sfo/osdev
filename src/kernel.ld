
MEMORY {
	KERNEL : ORIGIN = 1M, LENGTH = 15M
}

SECTIONS {
	.text : {
		*(.text)
		*(.text.*)
		. = ALIGN(4K);
	} > KERNEL
	.rodata : {
		*(.rodata)
		*(.rodata.*)
		. = ALIGN(4K);
	} > KERNEL
	.data : {
		*(.data)
		*(.data.*)
		. = ALIGN(4K);
	} > KERNEL
	.bss : {
		*(COMMON)
		*(.bss)
		*(.bss.*)
	} > KERNEL
}
