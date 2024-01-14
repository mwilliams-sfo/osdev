
.globl intr_handler_table

intr_dispatch:
	movl intr_handler_table(,%eax,4), %ebx
	testl %ebx, %ebx
	jz 0f
	push %eax
	call *%ebx
	addl $4, %esp
0:	popal
	iret

// explicit 32-bit relative jump
.macro jmp32 addr
	.byte 0xe9
	.long \addr - . - 4
.endm

.macro isr_thunk n
	pushal
	movl $0x\n, %eax
	jmp32 intr_dispatch
.endm

.globl intr_vector_base
intr_vector_base:
	isr_thunk 00
intr_vector_01:
	.irp y,1,2,3,4,5,6,7,8,9,a,b,c,d,e,f
	isr_thunk 0\y
	.endr
	.irp x,1,2,3,4,5,6,7,8,9,a,b,c,d,e,f
	.irp y,0,1,2,3,4,5,6,7,8,9,a,b,c,d,e,f
	isr_thunk \x\y
	.endr
	.endr

.globl intr_thunk_size
intr_thunk_size:
	.long intr_vector_01 - intr_vector_base
