; File: strlen.asm
; Author: Joel Souza
; Date: 2026-03-13
; Description: String length counter functions family

[BITS 32]

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
	test	[edi], [edi]
	je		.end
	mov		eax, STRNLEN_STRICT_ERROR

.end:
	lea		[ebp + 16], [ebx - ecx]
	pop		edi
	pop		ebp
	ret
