; File: strlen.asm
; Author: Joel Souza
; Date: 2026-03-13
; Description: String length counter functions family

[BITS 32]

%include "errno.inc"

global	strnlen_strict

section	.asm
strnlen_strict:
	push	ebp
	mov		ebp, esp
	push	edi

	mov		edi, [ebp + 8]
	mov		edx, [ebp + 12]
	xor		eax, eax
	xor		ecx, ecx

.loop:
	scasb
	jz		.end
	cmp		ecx, edx
	jz		.invalid_len
	add		ecx, 1
	jmp		.loop

.invalid_len:
	mov		eax, -ENAMETOOLONG

.end:
	mov		edx, [ebp + 16]
	mov		[edx], ecx
	pop		edi
	pop		ebp
	ret
