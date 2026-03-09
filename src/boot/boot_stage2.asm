; File: boot_stage2.asm
; Author: Joel Souza
; Date: 2026-03-01
; Description: Once MBR has a limited size to boot code (only 446 bytes),
;			   the actual code to load kernel from filesystem will reside
;			   here in the sector 1 until sector 2047 (~1MB MBR gap)

[BITS 32]

%include "string/string.inc"

section	.text
boot_stage2:
	push	7
	push	SRC
	push	DST
	call	memcpy
	add		esp, 12
	jmp	$

section	.data
DST: times 8 db 0
SRC: db "Hello world"
