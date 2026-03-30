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
	push	'.'
	push	dword [src_size]
	push	src
	push	dest
	call	memicpy8
	add		esp, 16

	jmp		$

section	.data
src:
	db "0123456789abcdef"
.end:

src_size: dd $ - src
dest: times (src.end - src) * 2 db 0
