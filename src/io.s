
.global inpb
inpb:
	push %ebp
	movl %esp, %ebp

	movl 8(%ebp), %edx
	xorl %eax, %eax
	inb %dx, %al

	pop %ebp
	ret

.global inpw
inpw:
	push %ebp
	movl %esp, %ebp

	movl 8(%ebp), %edx
	xorl %eax, %eax
	inw %dx, %ax

	pop %ebp
	ret

.global outb
outb:
	push %ebp
	movl %esp, %ebp

	movl 8(%ebp), %edx
	movl 12(%ebp), %eax
	outb %al, %dx

	pop %ebp
	ret

.global outw
outw:
	push %ebp
	movl %esp, %ebp

	movl 8(%ebp), %edx
	movl 12(%ebp), %eax
	outw %ax, %dx

	pop %ebp
	ret
