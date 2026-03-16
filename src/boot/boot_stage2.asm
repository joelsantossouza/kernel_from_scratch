; File: boot_stage2.asm
; Author: Joel Souza
; Date: 2026-03-01
; Description: Once MBR has a limited size to boot code (only 446 bytes),
;			   the actual code to load kernel from filesystem will reside
;			   here in the sector 1 until sector 2047 (~1MB MBR gap)

[BITS 32]

%include "string/string.inc"

MOVE_BYTES equ 5

section	.text
boot_stage2:
	mov		eax, dword [size]
	sub		eax, MOVE_BYTES
	push	eax
	push	string
	push	string
	call	memmove
	add		esp, 12
	jmp	$

section	.data
string: db "Joel Santos Souza"
size: dd $ - string
