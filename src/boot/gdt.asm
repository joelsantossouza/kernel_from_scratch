; File: gdt.asm
; Author: Joel Souza
; Date: 2026-03-01
; Description: Initial setup of Global Descriptor Table to
;			   enter in Protected Mode

[BITS 16]

gdt:
.null:
	dq	0x0

.kernel_code:
	dw	0xffff
	dw	0x0
	db	0x0
	db	10011010b
	db	11001111b
	db	0x0

.kernel_data:
	dw	0xffff
	dw	0x0
	db	0x0
	db	10011010b
	db	11001111b
	db	0x0
.end:

gdt_descriptor:
	dw	gdt.end - gdt - 1
	dd	gdt
