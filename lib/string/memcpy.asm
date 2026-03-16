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

	mov		edi, [ebp + 8]
	mov		esi, [ebp + 12]
	cmp		edi, esi
	jb		.forward_copy

.backward_copy:
	std
	mov		eax, [ebp + 16]
	add		edi, eax
	add		esi, eax

.copy_dword:
	mov		ecx, eax
	shr		ecx, INT32_SHIFT
	rep		movsd

.copy_byte:
	mov		ecx, eax
	and		ecx, INT32_MASK
	rep		movsb
	mov		eax, edi
	cld
	jmp		.end

.forward_copy:
	push	dword [ebp + 16]
	push	esi
	push	edi
	call	memcpy
	add		esp, 12

.end:
	pop		esi
	pop		edi
	pop		ebp
	ret
