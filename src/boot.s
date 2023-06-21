.org 0
.code16
	jmp 1f
	nop
.org 0x3e
1:	cli
	ljmpw $0x7c0, $2f
2:	movw %cs, %ax
	movw %ax, %ds
	movw %ax, %es
	movw %ax, %sp
	xorw %ax, %ax
	movw %ax, %ss
	sti

	movw $str, %si
	call print
	jmp .

print:	lodsb
	testb %al, %al
	jz 1f
	call putc
	jmp print
1:	ret

putc:	movb $0xe, %ah
	int $0x10
	ret

str:
.asciz "Done"

.org 510
.word 0xaa55
