
.globl paging_set_directory
.type paging_set_directory,@function
paging_set_directory:
	push %ebp
	movl %esp, %ebp

	movl 8(%ebp), %eax
	movl %eax, %cr3

	pop %ebp
	ret

.globl paging_enable
.type paging_enable,@function
paging_enable:
	movl %cr0, %eax
	orl $0x80000000, %eax
	movl %eax, %cr0
	ret
