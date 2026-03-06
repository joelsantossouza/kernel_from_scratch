; File: boot_stage2.asm
; Author: Joel Souza
; Date: 2026-03-01
; Description: Once MBR has a limited size to boot code (only 446 bytes),
;			   the actual code to load kernel from filesystem will reside
;			   here in the sector 1 until sector 2047 (~1MB MBR gap)

[BITS 32]

%include "drivers/disk/ata/ata.inc"

section	.text
boot_stage2:
	; WARNING: DELETE ME
	push	1
	push	BUF
	push	2
	push	0
	call	disk_ata_read
	add		esp, 16

.halt:
	hlt
	jmp		.halt

section	.data
BUF: times 509 db 0 ; WARNING: DELETE ME
