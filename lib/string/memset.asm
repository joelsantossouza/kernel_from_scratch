; File: memset.asm
; Author: Joel Souza
; Date: 2026-04-10
; Description: Memory set functions family

[BITS 32]

%include "stddef.inc"

global	memset
global	memsetw
global	memset_pattern

extern	mempcpy
extern	memcpy

section	.asm

; NAME
; 	memset, memsetw, memset_pattern - Memory set functions
;
; SYNOPSIS
; 	void	*memset(void *dest, int c, uint32_t n);
; 	void	*memsetw(void *dest, int c, uint32_t n);
; 	void	*memset_pattern(void *dest, const void *pattern,
; 								uint32_t pattern_size, uint32_t n);
;
; DESCRIPTION
; 	memset()
; 		Sets the first n bytes of memory area pointed to
; 		by dest with the constant byte c.
;
; 	memsetw()
; 		Sets the first n 16-bit words of memory area pointed
; 		to by dest with the constant word c.
;
; 	memset_pattern()
; 		Sets the first n bytes of memory area pointed to by
; 		dest with the repetition of the pattern.
;
; 		If n is not multiple of pattern_size, the last
; 		repetition is trucated.
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


memsetw:
	push	ebp
	mov		ebp, esp
	push	edi
	cld

	mov		edi, [ebp + 8]
	mov		edx, [ebp + 12]
	mov		ax, dx
	shl		eax, 16
	mov		ax, dx
	mov		edx, [ebp + 16]

.set_dword:
	mov		ecx, edx
	shr		ecx, WORD_LOG2
	rep		stosd

.set_word:
	mov		ecx, edx
	and		ecx, WORD_MASK
	rep		stosw

.return:
	mov		eax, [ebp + 8]
	pop		edi
	pop		ebp
	ret


memset_pattern:
	push	ebp
	mov		ebp, esp
	push	edi
	push	esi
	push	ebx

	mov		eax, [ebp + 8]
	mov		edi, [ebp + 12]
	mov		esi, [ebp + 16]
	mov		ebx, [ebp + 20]

	test	esi, esi
	jz		.return

.set_full_pattern:
	cmp		ebx, esi
	jb		.set_partial_pattern
	push	esi
	push	edi
	push	eax
	call	mempcpy
	add		esp, 12
	sub		ebx, esi
	jmp		.set_full_pattern

.set_partial_pattern:
	push	ebx
	push	edi
	push	eax
	call	memcpy
	add		esp, 12

.return:
	mov		eax, [ebp + 8]
	pop		ebx
	pop		esi
	pop		edi
	pop		ebp
	ret
