.org 0
.code16
mov $0xe, %ah
mov $'.', %al
int $0x10
jmp .

.org 510
.byte 0x55, 0xaa
