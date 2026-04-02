; File: memcmp.asm
; Author: Joel Souza
; Date: 2026-03-17
; Description: Memory comparision functions family

[BITS 32]

%include "stddef.inc"

global	memcmp

section	.asm
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
