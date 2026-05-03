; File: strlen.asm
; Author: Joel Souza
; Date: 2026-03-13
; Description: String length counter functions family

[BITS 32]

%include "errno.inc"

global	strlen
global	strnlen_strict

section	.asm

; NAME
; 	strlen, strnlen_strict - Calculate string length
;
; SYNOPSIS
; 	uint32_t	strlen(const char *s);
; 	int			strnlen_strict(const char *s, uint32_t n, uint32_t *len);
;
; DESCRIPTION
; 	Calculates the length of the string by counting the number of
; 	characters until the null terminator '\0' is reached.
;
; 	The string must be null-terminated.
; 	The returned length does not include the null terminator.
;
; 	strnlen_strict()
; 		Calculates the length of the string up to a maximum of n bytes.
;
; 		The computed length is stored in *len. If the null terminator
; 		is found before reaching n bytes, the full length is stored.
;
; 		If the string exceeds n bytes without encountering a null
; 		terminator, *len is set to n.
;
; RETURN VALUE
; 	strlen() returns the length of the string.
;
; 	strnlen_strict() returns 0 if the string length is less than or
; 	equal to n. Returns -ENAMETOOLONG if the string exceeds n bytes.
strlen:
	push	ebp
	mov		ebp, esp

	mov		edx, [ebp + 8]
	xor		eax, eax

.loop:
	cmp		byte [edx + eax], 0
	jz		.return
	add		eax, 1
	jmp		.loop

.return:
	pop		ebp
	ret

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
