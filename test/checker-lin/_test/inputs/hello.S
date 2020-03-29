.section .data
str:
	.ascii "Hello, world!\n"
str_len = . - str

.section .text

.global _start
_start:
	mov str, %al
	mov $1, %ebx
	mov $str, %ecx
	mov $str_len, %edx
	mov $4, %eax

	int $0x80

	mov $0, %ebx
	mov $1, %eax
	int $0x80
