; File: memset.asm
; Author: Joel Souza
; Date: 2026-04-10
; Description: Memory set functions family

[BITS 32]

%include "stddef.inc"

global	memset

section	.asm

; NAME
; 	memset - Memory set function
;
; SYNOPSIS
; 	void	*memset(void *dest, int c, uint32_t n);
;
; DESCRIPTION
; 	This function set the first n bytes of memory area
; 	pointed to by dest with the constant byte c.
;
; RETURN VALUE
; 	Returns a pointer to the memory area pointed to by dest.
memset:
	push	ebp
	mov		ebp, esp
	push	edi
	cld

	mov		edi, [ebp + 8]
	mov		eax, [ebp + 12]
	mov		edx, [ebp + 16]

.set_dword:
	mov		ah, al
	mov		cx, ax
	shl		eax, 16
	mov		ax, cx
	mov		ecx, edx
	shr		ecx, DWORD_LOG2
	rep		stosd

.set_byte:
	mov		ecx, edx
	and		ecx, DWORD_MASK
	rep		stosb

.return:
	mov		eax, [ebp + 8]
	pop		edi
	pop		ebp
	ret
