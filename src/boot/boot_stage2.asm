; File: boot_stage2.asm
; Author: Joel Souza
; Date: 2026-03-01
; Description: Once MBR has a limited size to boot code (only 446 bytes),
;			   the actual code to load kernel from filesystem will reside
;			   here in the sector 1 until sector 2047 (~1MB MBR gap)

[BITS 32]

%include "drivers/disk/ata/ata.inc"
%include "drivers/io/io.inc"

section	.boot_stage2
boot_stage2:
	push	1
	push	BUF
	push	4
	push	0
	call	disk_ata_read
	add		esp, 16

	push	BUF
	call	print
	add		esp, 4
	jmp		$

print:
	push	ebp
	mov		ebp, esp

	mov		ecx, 0xb8000
	mov		ebx, [ebp + 8]
	mov		ah, 0xf
.loop:
	mov		al, byte [ebx]
	cmp		al, 0
	je		.end
	mov		word [ecx], ax
	add		ebx, 1
	add		ecx, 2
	jmp		.loop
.end:
	pop		ebp
	ret

BUF: times 513 db 0
