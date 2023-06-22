.org 0

.set boot_base, 0x7c00
.set code_seg, gdt_code - gdt
.set data_seg, gdt_data - gdt
.set stack_base, 0x200000

.code16
	jmp 1f
	nop
.org 0x3e
1:	cli
	ljmpw $0, $boot_base + 2f
2:	movw %cs, %ax
	movw %ax, %ds
	movw %ax, %es
	movw %ax, %ss
	movw $0x7c00, %sp

	lgdtw boot_base + gdt_desc
	movl %cr0, %eax
	orl $1, %eax
	movl %eax, %cr0
	ljmpw $code_seg, $boot_base + 3f
.code32
3:	movw $data_seg, %ax
	movw %ax, %ds
	movw %ax, %es
	movw %ax, %fs
	movw %ax, %gs
	movw %ax, %ss
	movl $stack_base, %ebp
	movl %ebp, %esp
	jmp .

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
.long (0x7c00 + gdt)

.org 510
.word 0xaa55
