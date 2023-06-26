
.set boot_base, 0x7c00
.set code_sel, gdt_code - gdt
.set data_sel, gdt_data - gdt
.set stack_base, 0x200000
.set kernel_base, 0x100000

.org 0
.code16
	jmp 0f
	nop
.org 0x3e
0:	cld
	cli
	ljmpw $0, $boot_base + 1f
1:	movw %cs, %ax
	movw %ax, %ds
	movw %ax, %es
	movw %ax, %ss
	movw $0x7c00, %sp
	sti

	cli
	call enable_a20
	lgdtw boot_base + gdt_desc
	movl %cr0, %eax
	orl $1, %eax
	movl %eax, %cr0
	ljmpw $code_sel, $boot_base + init32

enable_a20:
	call test_a20
	je .
	ret

test_a20:
	push %ax
	push %bx
	push %ds
	movw $0xffff, %ax
	mov %ax, %ds
	movw $0xff, %ax
	movw %ax, 0x500
	movw %ds:0x510, %bx
	cmpw %ax, %bx
	jne 0f
	movw $0xff00, %ax
	movw %ax, 0x500
	movw %ds:0x510, %bx
	cmpw %ax, %bx
0:	pop %ds
	pop %bx
	pop %ax
	ret

.code32
init32:
	movw $data_sel, %ax
	movw %ax, %ds
	movw %ax, %es
	movw %ax, %fs
	movw %ax, %gs
	movw %ax, %ss
	movl $stack_base, %ebp
	movl %ebp, %esp

	movl $1, %eax
	movl $2, %ecx
	movl $kernel_base, %edi
0:	call ata_lba28_read
	testb $0x21, %al
	jnz .
	ljmpl $code_sel, $kernel_base

/*
eax - logical block address
ecx  - sector count
edi - destination address

returns:
al  - status register
*/
ata_lba28_read:
	call ata_lba28_read_cmd
	testb $0x21, %al
	jnz 2f
1:	call ata_lba28_read_xfer
	testb $0x21, %al
	jnz 2f
	loop 1b
2:	ret

/*
eax - logical block address
cl  - sector count

returns:
al  - status register
*/
ata_lba28_read_cmd:
	push %ebx
	push %ecx
	push %edx

	movl %eax, %ebx
	movw $0x3f6, %dx
0:	inb %dx, %al
	testb $0x21, %al
	jnz 1f
	testb $0x80, %al
	jnz 0b

	movl %ebx, %eax
	shrl $24, %eax
	orb $0xe0, %al
	movw $0x1f6, %dx
	outb %al, %dx
	movw $0x1f2, %dx
	movb %cl, %al
	outb %al, %dx
	movl %ebx, %eax
	incw %dx
	outb %al, %dx
	shrl $8, %eax
	incw %dx
	outb %al, %dx
	shrl $8, %eax
	incw %dx
	outb %al, %dx
	movb $0x20, %al
	incw %dx
	incw %dx
	outb %al, %dx

	movw $0x3f6, %dx
	inb %dx, %al

1:	pop %edx
	pop %ecx
	pop %ebx
	ret

/*
edi - destination address

returns
al  - status register
edi - updated destination address
*/
ata_lba28_read_xfer:
	push %ecx
	push %edx

	movw $0x3f6, %dx
0:	inb %dx, %al
	testb $0x21, %al
	jnz 2f
	testb $8, %al
	je 0b

	movl $0x100, %ecx
	movw $0x1f0, %dx
	rep
	insw

	movw $0x3f6, %dx
1:	inb %dx, %al
	testb $0x21, %al
	jnz 2f
	testb $8, %al
	jnz 1b

2:	pop %edx
	pop %ecx
	ret

.macro gdt_ent base, limit, access, flags
.word (\limit & 0xffff)
.word (\base & 0xffff)
.byte ((\base >> 16) & 0xff)
.byte \access
.byte (((\flags << 4) & 0xf0) | ((\limit >> 16) & 0xf))
.byte ((\base >> 24) & 0xff)
.endm

.align 4
gdt:
gdt_ent 0, 0, 0, 0
gdt_code:
gdt_ent 0, 0xfffff, 0b10011010, 0b1100
gdt_data:
gdt_ent 0, 0xfffff, 0b10010010, 0b1100
gdt_end:

gdt_desc:
.word (gdt_end - gdt - 1)
.long (boot_base + gdt)

.org 0x1fe
.word 0xaa55
