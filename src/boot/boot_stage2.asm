; File: boot_stage2.asm
; Author: Joel Souza
; Date: 2026-03-01
; Description: Once MBR has a limited size to boot code (only 446 bytes),
;			   the actual code to load kernel from filesystem will reside
;			   here in the sector 1 until sector 2047 (~1MB MBR gap)

[BITS 32]

%include "drivers/disk/ata/ata.inc"
%include "fs/vfs/vfs_partition.inc"
%include "fs/fat/fat.inc"

section	.text
boot_stage2:
	; Init partition
	push	partition0
	push	0
	push	disk
	call	vfs_partition_init
	add		esp, 12

	push	file
	push	file_path
	push	partition0
	call	fat_file_open
	add		esp, 12

	jmp	$

section	.data
file_path: db "boot/kernel"
file: times 44 db 0

partition0: times 1024 db 0

ata_driver:
	dd disk_ata_read
	dd disk_ata_to_errno

disk:
	dd ata_driver
	db 0
