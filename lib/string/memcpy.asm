; File: memcpy.asm
; Author: Joel Souza
; Date: 2026-03-09
; Description: Copy n bytes from source memory to destination memory

[BITS 32]

%define INT32_SIZE	4
%define INT32_SHIFT	2
%assign INT32_MASK	INT32_SIZE - 1

global	mempcpy
global	memcpy

section	.asm
mempcpy:
	push	ebp
	mov		ebp, esp
	push	edi
	push	esi

	mov		edi, [ebp + 8]
	mov		esi, [ebp + 12]
	mov		eax, [ebp + 16]
	cld

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
