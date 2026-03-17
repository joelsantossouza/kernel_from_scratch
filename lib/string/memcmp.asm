; File: memcmp.asm
; Author: Joel Souza
; Date: 2026-03-17
; Description: Memory comparision functions family

[BITS 32]

%include "stdint.inc"

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
	shr		ecx, INT32_SHIFT
	repz	cmpsd
	jz		.compare_byte
	sub		esi, INT32_BYTES
	sub		edi, INT32_BYTES
	mov		ecx, INT32_BYTES
	repz	cmpsb
	jb		.s1_smaller
	ja		.s1_greater

.compare_byte:
	mov		ecx, eax
	and		ecx, INT32_MASK
	repz	cmpsb
	jb		.s1_smaller
	ja		.s1_greater
	xor		eax, eax

.end:
	pop		esi
	pop		edi
	pop		ebp
	ret

.s1_smaller:
	mov		eax, -1
	jmp		.end

.s1_greater:
	mov		eax, 1
	jmp		.end
