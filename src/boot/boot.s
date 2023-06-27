
.set boot_base, 0x7c00
.set boot_magic, 0xaa55
.set bpb_len, 0x3e
.set code_sel, gdt_code - gdt
.set data_sel, gdt_data - gdt
.set stack_base, 0x200000
.set kernel_base, 0x100000
.set kernel_sectors, 16

.set port_ata_hd0_base, 0x1f0

.set ata_stat_err, 1
.set ata_stat_dq, 8
.set ata_stat_df, 0x20
.set ata_stat_bsy, 0x80

.set ata_cmd_read, 0x20

.org 0
.code16
	jmp bpb_len
	nop
.org bpb_len
	cld
	cli
	ljmpw $0, $boot_base + 0f
0:	movw %cs, %ax
	movw %ax, %ds
	movw %ax, %es
	movw %ax, %ss
	movw $boot_base, %sp

	call test_a20
	je .
	lgdtw boot_base + gdt_desc
	movl %cr0, %eax
	orl $1, %eax
	movl %eax, %cr0
	ljmpw $code_sel, $boot_base + init32

/*
returns:
zero flag set if A20 disabled
*/
test_a20:
	push %ax
	push %bx
	push %ds

	movw $0xffff, %ax
	movw %ax, %ds

	movw $0xff, %ax
	movw %ax, 0x500
	movw %ds:0x510, %bx
	cmpw %bx, %ax
	jne 0f
	shlw $8, %ax
	movw %ax, 0x500
	movw %ds:0x510, %bx
	cmpw %bx, %ax

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
	movl $kernel_sectors, %ecx
	movl $kernel_base, %edi
0:	call ata_lba28_read
	testb $(ata_stat_err | ata_stat_df), %al
	jnz .
	mov $kernel_base, %eax
	jmp *%eax

/*
eax - logical block address
ecx  - sector count
edi - destination address

returns:
al  - status register
*/
ata_lba28_read:
	call ata_lba28_read_cmd
	testb $(ata_stat_err | ata_stat_df), %al
	jnz 2f
1:	call ata_lba28_read_xfer
	testb $(ata_stat_err | ata_stat_df), %al
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
	movw $port_ata_hd0_base + 7, %dx
0:	inb %dx, %al
	testb $(ata_stat_err | ata_stat_df), %al
	jnz 1f
	testb $ata_stat_bsy, %al
	jnz 0b

	movl %ebx, %eax
	shrl $24, %eax
	orb $0xe0, %al
	movw $port_ata_hd0_base + 6, %dx
	outb %al, %dx
	movw $port_ata_hd0_base + 2, %dx
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
	movb $ata_cmd_read, %al
	incw %dx
	incw %dx
	outb %al, %dx

	movw $port_ata_hd0_base + 7, %dx
	inb %dx, %al

1:	pop %edx
	pop %ecx
	pop %ebx
	ret

/*
edi - destination address

returns:
al  - status register
edi - updated destination address
*/
ata_lba28_read_xfer:
	push %ecx
	push %edx

	movw $port_ata_hd0_base + 7, %dx
0:	inb %dx, %al
	testb $(ata_stat_err | ata_stat_df), %al
	jnz 2f
	testb $ata_stat_dq, %al
	je 0b

	movl $0x100, %ecx
	movw $port_ata_hd0_base, %dx
	rep
	insw

	movw $port_ata_hd0_base + 7, %dx
1:	inb %dx, %al
	testb $(ata_stat_err | ata_stat_df), %al
	jnz 2f
	testb $ata_stat_dq, %al
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
.word boot_magic
