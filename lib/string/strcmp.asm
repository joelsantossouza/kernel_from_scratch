; File: strcmp.asm
; Author: Joel Souza
; Date: 2026-03-16
; Description: String comparison functions family

[BITS 32]

global	strcmp

section	.asm
strcmp:
	push	ebp
	mov		ebp, esp

	mov		edx, [ebp + 8]
	mov		ecx, [ebp + 12]

.loop:
	movzx	eax, byte [edx]
	sub		al, [ecx]
	jnz		.return
	cmp		byte [edx], 0
	jz		.return
	add		edx, 1
	add		ecx, 1
	jmp		.loop

.return:
	movsx	eax, al
	pop		ebp
	ret
