; File: boot_stage2.asm
; Author: Joel Souza
; Date: 2026-03-01
; Description: Once MBR has a limited size to boot code (only 446 bytes),
;			   the actual code to load kernel from filesystem will reside
;			   here in the sector 1 until sector 2047 (~1MB MBR gap)

[BITS 32]

global boot_stage2

boot_stage2:
	jmp $

;extern disk_ata_read ; WARNING: DELETE ME
;
;section	.asm
;boot_stage2:
;	; WARNING: DELETE ME
;	push	1
;	push	BUF
;	push	2
;	push	0
;	call	disk_ata_read
;	add		esp, 16
;
;BUF: times 512 db 0 ; WARNING: DELETE ME
