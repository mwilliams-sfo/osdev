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

	movb $2, %ah
	movb $1, %al
	movb $0, %ch
	movb $2, %cl
	movb $0, %dh
	movb $0, %dl
	movw $0x200, %bx
	int $0x13
	jc 3f
	movw $0x200, %si
	call print
	jmp .
3:	movw diskerr, %si
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

diskerr:
.asciz "Disk error"

.org 510
.word 0xaa55

.org 512
.asciz "Welcome to Sector Two."
.org 1024
