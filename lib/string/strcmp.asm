; File: strcmp.asm
; Author: Joel Souza
; Date: 2026-03-16
; Description: String comparison functions family

[BITS 32]

global	strcmp

section	.asm

; NAME
; 	strcmp - Compare two strings
;
; SYNOPSIS
; 	int	strcmp(const char *s1, const char *s2);
;
; DESCRIPTION
; 	Compares the two null-terminated strings
; 	s1 and s2 based on its characters value.
;
; RETURN VALUE
; 	Returns an integer less than, equal to, or greater than zero if s1 is
; 	found, respectively, to be less than, to match, or be greater than s2.
strcmp:
	push	ebp
	mov		ebp, esp

	mov		edx, [ebp + 8]
	mov		ecx, [ebp + 12]

.loop:
	movzx	eax, byte [edx]
	sub		al, [ecx]
	jnz		.return
	cmp		byte [edx], 0
	jz		.return
	add		edx, 1
	add		ecx, 1
	jmp		.loop

.return:
	movsx	eax, al
	pop		ebp
	ret
