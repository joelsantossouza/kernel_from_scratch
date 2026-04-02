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
	mov		eax, edi
	pop		ebx
	pop		esi
	pop		edi
	pop		ebp
	ret
