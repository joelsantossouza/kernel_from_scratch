; File: strlen.asm
; Author: Joel Souza
; Date: 2026-03-13
; Description: String length counter functions family

[BITS 32]

%include "errno.inc"

section	.asm
strnlen_strict:
	push	ebp
	mov		ebp, esp
	push	edi

	xor		eax, eax
	mov		edi, [ebp + 8]
	mov		ebx, [ebp + 12]
	mov		ecx, ebx

	repnz	scasb
	cmp		byte [edi], 0
	je		.end
	mov		eax, -ENAMETOOLONG

.end:
	sub		ebx, ecx
	mov		[ebp + 16], ebx
	pop		edi
	pop		ebp
	ret
