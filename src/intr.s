
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

.macro isr_thunk n
.type intr_vector_\n,@function
.align 4
intr_vector_\n:
	pushal
	movl $\n, %eax
	jmp intr_dispatch
.endm

isr_thunk 0x00
isr_thunk 0x01
isr_thunk 0x02
isr_thunk 0x03
isr_thunk 0x04
isr_thunk 0x05
isr_thunk 0x06
isr_thunk 0x07
isr_thunk 0x08
isr_thunk 0x09
isr_thunk 0x0a
isr_thunk 0x0b
isr_thunk 0x0c
isr_thunk 0x0d
isr_thunk 0x0e
isr_thunk 0x0f
isr_thunk 0x10
isr_thunk 0x11
isr_thunk 0x12
isr_thunk 0x13
isr_thunk 0x14
isr_thunk 0x15
isr_thunk 0x16
isr_thunk 0x17
isr_thunk 0x18
isr_thunk 0x19
isr_thunk 0x1a
isr_thunk 0x1b
isr_thunk 0x1c
isr_thunk 0x1d
isr_thunk 0x1e
isr_thunk 0x1f
isr_thunk 0x20
isr_thunk 0x21
isr_thunk 0x22
isr_thunk 0x23
isr_thunk 0x24
isr_thunk 0x25
isr_thunk 0x26
isr_thunk 0x27
isr_thunk 0x28
isr_thunk 0x29
isr_thunk 0x2a
isr_thunk 0x2b
isr_thunk 0x2c
isr_thunk 0x2d
isr_thunk 0x2e
isr_thunk 0x2f
isr_thunk 0x30
isr_thunk 0x31
isr_thunk 0x32
isr_thunk 0x33
isr_thunk 0x34
isr_thunk 0x35
isr_thunk 0x36
isr_thunk 0x37
isr_thunk 0x38
isr_thunk 0x39
isr_thunk 0x3a
isr_thunk 0x3b
isr_thunk 0x3c
isr_thunk 0x3d
isr_thunk 0x3e
isr_thunk 0x3f
isr_thunk 0x40
isr_thunk 0x41
isr_thunk 0x42
isr_thunk 0x43
isr_thunk 0x44
isr_thunk 0x45
isr_thunk 0x46
isr_thunk 0x47
isr_thunk 0x48
isr_thunk 0x49
isr_thunk 0x4a
isr_thunk 0x4b
isr_thunk 0x4c
isr_thunk 0x4d
isr_thunk 0x4e
isr_thunk 0x4f
isr_thunk 0x50
isr_thunk 0x51
isr_thunk 0x52
isr_thunk 0x53
isr_thunk 0x54
isr_thunk 0x55
isr_thunk 0x56
isr_thunk 0x57
isr_thunk 0x58
isr_thunk 0x59
isr_thunk 0x5a
isr_thunk 0x5b
isr_thunk 0x5c
isr_thunk 0x5d
isr_thunk 0x5e
isr_thunk 0x5f
isr_thunk 0x60
isr_thunk 0x61
isr_thunk 0x62
isr_thunk 0x63
isr_thunk 0x64
isr_thunk 0x65
isr_thunk 0x66
isr_thunk 0x67
isr_thunk 0x68
isr_thunk 0x69
isr_thunk 0x6a
isr_thunk 0x6b
isr_thunk 0x6c
isr_thunk 0x6d
isr_thunk 0x6e
isr_thunk 0x6f
isr_thunk 0x70
isr_thunk 0x71
isr_thunk 0x72
isr_thunk 0x73
isr_thunk 0x74
isr_thunk 0x75
isr_thunk 0x76
isr_thunk 0x77
isr_thunk 0x78
isr_thunk 0x79
isr_thunk 0x7a
isr_thunk 0x7b
isr_thunk 0x7c
isr_thunk 0x7d
isr_thunk 0x7e
isr_thunk 0x7f
isr_thunk 0x80
isr_thunk 0x81
isr_thunk 0x82
isr_thunk 0x83
isr_thunk 0x84
isr_thunk 0x85
isr_thunk 0x86
isr_thunk 0x87
isr_thunk 0x88
isr_thunk 0x89
isr_thunk 0x8a
isr_thunk 0x8b
isr_thunk 0x8c
isr_thunk 0x8d
isr_thunk 0x8e
isr_thunk 0x8f
isr_thunk 0x90
isr_thunk 0x91
isr_thunk 0x92
isr_thunk 0x93
isr_thunk 0x94
isr_thunk 0x95
isr_thunk 0x96
isr_thunk 0x97
isr_thunk 0x98
isr_thunk 0x99
isr_thunk 0x9a
isr_thunk 0x9b
isr_thunk 0x9c
isr_thunk 0x9d
isr_thunk 0x9e
isr_thunk 0x9f
isr_thunk 0xa0
isr_thunk 0xa1
isr_thunk 0xa2
isr_thunk 0xa3
isr_thunk 0xa4
isr_thunk 0xa5
isr_thunk 0xa6
isr_thunk 0xa7
isr_thunk 0xa8
isr_thunk 0xa9
isr_thunk 0xaa
isr_thunk 0xab
isr_thunk 0xac
isr_thunk 0xad
isr_thunk 0xae
isr_thunk 0xaf
isr_thunk 0xb0
isr_thunk 0xb1
isr_thunk 0xb2
isr_thunk 0xb3
isr_thunk 0xb4
isr_thunk 0xb5
isr_thunk 0xb6
isr_thunk 0xb7
isr_thunk 0xb8
isr_thunk 0xb9
isr_thunk 0xba
isr_thunk 0xbb
isr_thunk 0xbc
isr_thunk 0xbd
isr_thunk 0xbe
isr_thunk 0xbf
isr_thunk 0xc0
isr_thunk 0xc1
isr_thunk 0xc2
isr_thunk 0xc3
isr_thunk 0xc4
isr_thunk 0xc5
isr_thunk 0xc6
isr_thunk 0xc7
isr_thunk 0xc8
isr_thunk 0xc9
isr_thunk 0xca
isr_thunk 0xcb
isr_thunk 0xcc
isr_thunk 0xcd
isr_thunk 0xce
isr_thunk 0xcf
isr_thunk 0xd0
isr_thunk 0xd1
isr_thunk 0xd2
isr_thunk 0xd3
isr_thunk 0xd4
isr_thunk 0xd5
isr_thunk 0xd6
isr_thunk 0xd7
isr_thunk 0xd8
isr_thunk 0xd9
isr_thunk 0xda
isr_thunk 0xdb
isr_thunk 0xdc
isr_thunk 0xdd
isr_thunk 0xde
isr_thunk 0xdf
isr_thunk 0xe0
isr_thunk 0xe1
isr_thunk 0xe2
isr_thunk 0xe3
isr_thunk 0xe4
isr_thunk 0xe5
isr_thunk 0xe6
isr_thunk 0xe7
isr_thunk 0xe8
isr_thunk 0xe9
isr_thunk 0xea
isr_thunk 0xeb
isr_thunk 0xec
isr_thunk 0xed
isr_thunk 0xee
isr_thunk 0xef
isr_thunk 0xf0
isr_thunk 0xf1
isr_thunk 0xf2
isr_thunk 0xf3
isr_thunk 0xf4
isr_thunk 0xf5
isr_thunk 0xf6
isr_thunk 0xf7
isr_thunk 0xf8
isr_thunk 0xf9
isr_thunk 0xfa
isr_thunk 0xfb
isr_thunk 0xfc
isr_thunk 0xfd
isr_thunk 0xfe
isr_thunk 0xff

.globl intr_vector_table
.type intr_vector_table,@object
.data
intr_vector_table:
.long intr_vector_0x00
.long intr_vector_0x01
.long intr_vector_0x02
.long intr_vector_0x03
.long intr_vector_0x04
.long intr_vector_0x05
.long intr_vector_0x06
.long intr_vector_0x07
.long intr_vector_0x08
.long intr_vector_0x09
.long intr_vector_0x0a
.long intr_vector_0x0b
.long intr_vector_0x0c
.long intr_vector_0x0d
.long intr_vector_0x0e
.long intr_vector_0x0f
.long intr_vector_0x10
.long intr_vector_0x11
.long intr_vector_0x12
.long intr_vector_0x13
.long intr_vector_0x14
.long intr_vector_0x15
.long intr_vector_0x16
.long intr_vector_0x17
.long intr_vector_0x18
.long intr_vector_0x19
.long intr_vector_0x1a
.long intr_vector_0x1b
.long intr_vector_0x1c
.long intr_vector_0x1d
.long intr_vector_0x1e
.long intr_vector_0x1f
.long intr_vector_0x20
.long intr_vector_0x21
.long intr_vector_0x22
.long intr_vector_0x23
.long intr_vector_0x24
.long intr_vector_0x25
.long intr_vector_0x26
.long intr_vector_0x27
.long intr_vector_0x28
.long intr_vector_0x29
.long intr_vector_0x2a
.long intr_vector_0x2b
.long intr_vector_0x2c
.long intr_vector_0x2d
.long intr_vector_0x2e
.long intr_vector_0x2f
.long intr_vector_0x30
.long intr_vector_0x31
.long intr_vector_0x32
.long intr_vector_0x33
.long intr_vector_0x34
.long intr_vector_0x35
.long intr_vector_0x36
.long intr_vector_0x37
.long intr_vector_0x38
.long intr_vector_0x39
.long intr_vector_0x3a
.long intr_vector_0x3b
.long intr_vector_0x3c
.long intr_vector_0x3d
.long intr_vector_0x3e
.long intr_vector_0x3f
.long intr_vector_0x40
.long intr_vector_0x41
.long intr_vector_0x42
.long intr_vector_0x43
.long intr_vector_0x44
.long intr_vector_0x45
.long intr_vector_0x46
.long intr_vector_0x47
.long intr_vector_0x48
.long intr_vector_0x49
.long intr_vector_0x4a
.long intr_vector_0x4b
.long intr_vector_0x4c
.long intr_vector_0x4d
.long intr_vector_0x4e
.long intr_vector_0x4f
.long intr_vector_0x50
.long intr_vector_0x51
.long intr_vector_0x52
.long intr_vector_0x53
.long intr_vector_0x54
.long intr_vector_0x55
.long intr_vector_0x56
.long intr_vector_0x57
.long intr_vector_0x58
.long intr_vector_0x59
.long intr_vector_0x5a
.long intr_vector_0x5b
.long intr_vector_0x5c
.long intr_vector_0x5d
.long intr_vector_0x5e
.long intr_vector_0x5f
.long intr_vector_0x60
.long intr_vector_0x61
.long intr_vector_0x62
.long intr_vector_0x63
.long intr_vector_0x64
.long intr_vector_0x65
.long intr_vector_0x66
.long intr_vector_0x67
.long intr_vector_0x68
.long intr_vector_0x69
.long intr_vector_0x6a
.long intr_vector_0x6b
.long intr_vector_0x6c
.long intr_vector_0x6d
.long intr_vector_0x6e
.long intr_vector_0x6f
.long intr_vector_0x70
.long intr_vector_0x71
.long intr_vector_0x72
.long intr_vector_0x73
.long intr_vector_0x74
.long intr_vector_0x75
.long intr_vector_0x76
.long intr_vector_0x77
.long intr_vector_0x78
.long intr_vector_0x79
.long intr_vector_0x7a
.long intr_vector_0x7b
.long intr_vector_0x7c
.long intr_vector_0x7d
.long intr_vector_0x7e
.long intr_vector_0x7f
.long intr_vector_0x80
.long intr_vector_0x81
.long intr_vector_0x82
.long intr_vector_0x83
.long intr_vector_0x84
.long intr_vector_0x85
.long intr_vector_0x86
.long intr_vector_0x87
.long intr_vector_0x88
.long intr_vector_0x89
.long intr_vector_0x8a
.long intr_vector_0x8b
.long intr_vector_0x8c
.long intr_vector_0x8d
.long intr_vector_0x8e
.long intr_vector_0x8f
.long intr_vector_0x90
.long intr_vector_0x91
.long intr_vector_0x92
.long intr_vector_0x93
.long intr_vector_0x94
.long intr_vector_0x95
.long intr_vector_0x96
.long intr_vector_0x97
.long intr_vector_0x98
.long intr_vector_0x99
.long intr_vector_0x9a
.long intr_vector_0x9b
.long intr_vector_0x9c
.long intr_vector_0x9d
.long intr_vector_0x9e
.long intr_vector_0x9f
.long intr_vector_0xa0
.long intr_vector_0xa1
.long intr_vector_0xa2
.long intr_vector_0xa3
.long intr_vector_0xa4
.long intr_vector_0xa5
.long intr_vector_0xa6
.long intr_vector_0xa7
.long intr_vector_0xa8
.long intr_vector_0xa9
.long intr_vector_0xaa
.long intr_vector_0xab
.long intr_vector_0xac
.long intr_vector_0xad
.long intr_vector_0xae
.long intr_vector_0xaf
.long intr_vector_0xb0
.long intr_vector_0xb1
.long intr_vector_0xb2
.long intr_vector_0xb3
.long intr_vector_0xb4
.long intr_vector_0xb5
.long intr_vector_0xb6
.long intr_vector_0xb7
.long intr_vector_0xb8
.long intr_vector_0xb9
.long intr_vector_0xba
.long intr_vector_0xbb
.long intr_vector_0xbc
.long intr_vector_0xbd
.long intr_vector_0xbe
.long intr_vector_0xbf
.long intr_vector_0xc0
.long intr_vector_0xc1
.long intr_vector_0xc2
.long intr_vector_0xc3
.long intr_vector_0xc4
.long intr_vector_0xc5
.long intr_vector_0xc6
.long intr_vector_0xc7
.long intr_vector_0xc8
.long intr_vector_0xc9
.long intr_vector_0xca
.long intr_vector_0xcb
.long intr_vector_0xcc
.long intr_vector_0xcd
.long intr_vector_0xce
.long intr_vector_0xcf
.long intr_vector_0xd0
.long intr_vector_0xd1
.long intr_vector_0xd2
.long intr_vector_0xd3
.long intr_vector_0xd4
.long intr_vector_0xd5
.long intr_vector_0xd6
.long intr_vector_0xd7
.long intr_vector_0xd8
.long intr_vector_0xd9
.long intr_vector_0xda
.long intr_vector_0xdb
.long intr_vector_0xdc
.long intr_vector_0xdd
.long intr_vector_0xde
.long intr_vector_0xdf
.long intr_vector_0xe0
.long intr_vector_0xe1
.long intr_vector_0xe2
.long intr_vector_0xe3
.long intr_vector_0xe4
.long intr_vector_0xe5
.long intr_vector_0xe6
.long intr_vector_0xe7
.long intr_vector_0xe8
.long intr_vector_0xe9
.long intr_vector_0xea
.long intr_vector_0xeb
.long intr_vector_0xec
.long intr_vector_0xed
.long intr_vector_0xee
.long intr_vector_0xef
.long intr_vector_0xf0
.long intr_vector_0xf1
.long intr_vector_0xf2
.long intr_vector_0xf3
.long intr_vector_0xf4
.long intr_vector_0xf5
.long intr_vector_0xf6
.long intr_vector_0xf7
.long intr_vector_0xf8
.long intr_vector_0xf9
.long intr_vector_0xfa
.long intr_vector_0xfb
.long intr_vector_0xfc
.long intr_vector_0xfd
.long intr_vector_0xfe
.long intr_vector_0xff
