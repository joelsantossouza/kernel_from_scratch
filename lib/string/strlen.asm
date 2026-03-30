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
	cld

	mov		edi, [ebp + 8]
	mov		edx, [ebp + 12]
	xor		eax, eax
	xor		ecx, ecx

.loop:
	scasb
	jz		.return
	cmp		ecx, edx
	jz		.invalid_len
	add		ecx, 1
	jmp		.loop

.invalid_len:
	mov		eax, -ENAMETOOLONG

.return:
	mov		edx, [ebp + 16]
	mov		[edx], ecx
	pop		edi
	pop		ebp
	ret
