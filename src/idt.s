
.globl idt_load
.type idt_load,@function
idt_load:
	push %ebp
	movl %esp, %ebp

	movl 8(%ebp), %ebx
	lidt (%ebx)

	pop %ebp
	ret
