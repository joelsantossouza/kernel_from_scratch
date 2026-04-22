; File: memcpy.asm
; Author: Joel Souza
; Date: 2026-03-09
; Description: Copy n bytes from source memory to destination memory

[BITS 32]

%include "stddef.inc"

global	mempcpy
global	memcpy
global	memmove
global	memicpy8

section	.asm

; NAME
; 	memcpy, mempcpy, memmove, memicpy8 - Memory copy functions
;
; SYNOPSIS
; 	void	*memcpy(void *dest, const void *src, uint32_t n);
; 	void	*mempcpy(void *dest, const void *src, uint32_t n);
; 	void	*memmove(void *dest, const void *src, uint32_t n);
; 	void	*memicpy8(void *dest, const void *src, uint32_t n, uint8_t attr);
;
; DESCRIPTION
; 	These functions copy n bytes from the memory area pointed to by src
; 	to the memory area pointed to by dest.
;
; memcpy(), mempcpy(), and memicpy8():
; 	The behavior is undefined if the source and destination memory
; 	regions overlap.
;
; memmove():
; 	Guarantees correct copying even if the source and destination
; 	memory regions overlap. It handles overlap by adjusting the
; 	copy direction.
;
; memicpy8():
; 	Copies bytes from src to dest while interleaving each byte
; 	with the attribute byte 'attr'.
;
; 	Resulting layout: {src[0], attr, src[1], attr, ...}
;
; RETURN VALUE
; 	memcpy(), memmove(), and memicpy8() return dest.
;
; 	mempcpy() returns dest + n.
mempcpy:
	push	ebp
	mov		ebp, esp
	push	edi
	push	esi
	cld

	mov		edi, [ebp + 8]
	mov		esi, [ebp + 12]
	mov		eax, [ebp + 16]

.copy_dword:
	mov		ecx, eax
	shr		ecx, DWORD_LOG2
	rep		movsd

.copy_byte:
	mov		ecx, eax
	and		ecx, DWORD_MASK
	rep		movsb

.return:
	mov		eax, edi
	pop		esi
	pop		edi
	pop		ebp
	ret

memcpy:
	push	ebp
	mov		ebp, esp

	push	dword [ebp + 16]
	push	dword [ebp + 12]
	push	dword [ebp + 8]
	call	mempcpy
	add		esp, 12

	mov		eax, [ebp + 8]
	pop		ebp
	ret

memmove:
	push	ebp
	mov		ebp, esp
	push	edi
	push	esi

	mov		esi, [ebp + 12]
	mov		edx, [ebp + 8]
	mov		edi, edx
	cmp		edi, esi
	jbe		.copy_forward

.copy_backward:
	std
	mov		eax, [ebp + 16]
	lea		edi, [edi + eax - 1]
	lea		esi, [esi + eax - 1]

.copy_byte:
	mov		ecx, eax
	and		ecx, DWORD_MASK
	rep		movsb

.copy_dword:
	sub		edi, 3
	sub		esi, 3
	mov		ecx, eax
	shr		ecx, DWORD_LOG2
	rep		movsd
	mov		eax, edx
	cld

.return:
	pop		esi
	pop		edi
	pop		ebp
	ret

.copy_forward:
	push	dword [ebp + 16]
	push	esi
	push	edi
	call	memcpy
	add		esp, 12
	jmp		.return

memicpy8:
	push	ebp
	mov		ebp, esp
	push	edi
	push	esi
	push	ebx

	mov		edi, [ebp + 8]
	mov		esi, [ebp + 12]
	mov		ecx, [ebp + 16]
	mov		eax, [ebp + 20]
	mov		ebx, eax
	shl		ebx, 24
	mov		bh, al

	jmp		.while1
.interleave_dword:
	mov		eax, dword [esi]
	mov		edx, eax

	shl		eax, 8
	shr		ax, 8
	and		eax, 0x00FFFFFF
	or		eax, ebx

	shr		edx, 8
	shr		dx, 8
	or		edx, ebx

	mov		dword [edi], eax
	mov		dword [edi + DWORD], edx
	add		edi, QWORD
	add		esi, DWORD
	sub		ecx, DWORD
.while1:
	cmp		ecx, DWORD
	jae		.interleave_dword

	mov		ah, bh
	jmp		.while2
.interleave_byte:
	mov		al, byte [esi]
	mov		word [edi], ax
	add		edi, WORD
	add		esi, BYTE
	sub		ecx, BYTE
.while2:
	cmp		ecx, BYTE
	jae		.interleave_byte

.return:
	mov		eax, [ebp + 8]
	pop		ebx
	pop		esi
	pop		edi
	pop		ebp
	ret
