.org 0
.code16
	movw $0x7c0, %ax
	movw %ax, %ds
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
