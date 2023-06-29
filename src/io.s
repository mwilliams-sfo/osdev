
.global inpb
.type inpb,@function
inpb:
	push %ebp
	movl %esp, %ebp

	movl 8(%ebp), %edx
	xorl %eax, %eax
	inb %dx, %al

	pop %ebp
	ret

.global inpw
.type inpw,@function
inpw:
	push %ebp
	movl %esp, %ebp

	movl 8(%ebp), %edx
	xorl %eax, %eax
	inw %dx, %ax

	pop %ebp
	ret

.global outb
.type outb,@function
outb:
	push %ebp
	movl %esp, %ebp

	movl 8(%ebp), %edx
	movl 12(%ebp), %eax
	outb %al, %dx

	pop %ebp
	ret

.global outw
.type outw,@function
outw:
	push %ebp
	movl %esp, %ebp

	movl 8(%ebp), %edx
	movl 12(%ebp), %eax
	outw %ax, %dx

	pop %ebp
	ret
