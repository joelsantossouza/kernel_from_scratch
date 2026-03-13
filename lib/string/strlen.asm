; File: strlen.asm
; Author: Joel Souza
; Date: 2026-03-13
; Description: String length counter functions family

[BITS 32]

section	.asm
strnlen_strict:
	push	ebp
	mov		ebp, esp
	
	pop		ebp
	ret
