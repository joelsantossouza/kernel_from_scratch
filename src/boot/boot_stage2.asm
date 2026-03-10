; File: boot_stage2.asm
; Author: Joel Souza
; Date: 2026-03-01
; Description: Once MBR has a limited size to boot code (only 446 bytes),
;			   the actual code to load kernel from filesystem will reside
;			   here in the sector 1 until sector 2047 (~1MB MBR gap)

[BITS 32]

%include "drivers/disk/vdl/vdl.inc"
%include "drivers/disk/ata/ata.inc"

section	.text
boot_stage2:
	push	512
	push	buffer1
	push	10240
	push	disk
	call	disk_vdl_read
	add		esp, 16

	push	512
	push	buffer2
	push	10240
	push	disk
	call	disk_vdl_read
	add		esp, 16

	jmp	$

section	.data
driver:
	dd disk_ata_read

disk:
	dd driver
	db 0

buffer1: times 512 db 0
buffer2: times 512 db 0
