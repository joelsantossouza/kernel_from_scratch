; File: memcmp.asm
; Author: Joel Souza
; Date: 2026-03-17
; Description: Memory comparision functions family

[BITS 32]

%include "stddef.inc"

global	memcmp

section	.asm

; NAME
; 	memcmp - Memory comparison function
;
; SYNOPSIS
; 	int	memcmp(const void *s1, const void *s2, uint32_t n);
;
; DESCRIPTION
; 	Compares the first n bytes of the memory regions pointed
; 	to by s1 and s2.
;
; 	If n is zero, the function returns zero.
;
; RETURN VALUE
; 	Returns an integer less than, equal to, or greater than zero
; 	if the first differing byte in s1 is less than, equal to,
; 	or greater than the corresponding byte in s2.
memcmp:
	push	ebp
	mov		ebp, esp
	push	edi
	push	esi
	cld

	mov		esi, [ebp + 8]
	mov		edi, [ebp + 12]
	mov		eax, [ebp + 16]

.compare_dword:
	mov		ecx, eax
	shr		ecx, DWORD_LOG2
	repz	cmpsd
	jz		.compare_byte
	sub		esi, DWORD
	sub		edi, DWORD
	mov		ecx, DWORD
	repz	cmpsb
	jb		.s1_smaller
	ja		.s1_greater

.compare_byte:
	mov		ecx, eax
	and		ecx, DWORD_MASK
	repz	cmpsb
	jb		.s1_smaller
	ja		.s1_greater
	xor		eax, eax

.return:
	pop		esi
	pop		edi
	pop		ebp
	ret

.s1_smaller:
	mov		eax, -1
	jmp		.return

.s1_greater:
	mov		eax, 1
	jmp		.return
