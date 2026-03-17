; File: memcpy.asm
; Author: Joel Souza
; Date: 2026-03-09
; Description: Copy n bytes from source memory to destination memory

[BITS 32]

%include "stdint.inc"

global	mempcpy
global	memcpy
global	memmove

section	.asm
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
	shr		ecx, INT32_SHIFT
	rep		movsd

.copy_byte:
	mov		ecx, eax
	and		ecx, INT32_MASK
	rep		movsb

.end:
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
	jbe		.forward_copy

.backward_copy:
	std
	mov		eax, [ebp + 16]
	lea		edi, [edi + eax - 1]
	lea		esi, [esi + eax - 1]

.copy_byte:
	mov		ecx, eax
	and		ecx, INT32_MASK
	rep		movsb

.copy_dword:
	sub		edi, 3
	sub		esi, 3
	mov		ecx, eax
	shr		ecx, INT32_SHIFT
	rep		movsd
	mov		eax, edx
	cld

.end:
	pop		esi
	pop		edi
	pop		ebp
	ret

.forward_copy:
	push	dword [ebp + 16]
	push	esi
	push	edi
	call	memcpy
	add		esp, 12
	jmp		.end
