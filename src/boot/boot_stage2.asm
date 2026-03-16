; File: boot_stage2.asm
; Author: Joel Souza
; Date: 2026-03-01
; Description: Once MBR has a limited size to boot code (only 446 bytes),
;			   the actual code to load kernel from filesystem will reside
;			   here in the sector 1 until sector 2047 (~1MB MBR gap)

[BITS 32]

%include "drivers/disk/ata/ata.inc"
%include "fs/vfs/vfs_mount.inc"

section	.text
boot_stage2:
	push	mount_path
	push	0
	push	disk
	call	vfs_mount
	add		esp, 12
	jmp	$

section	.data
mount_path:	db "/boot/kernel", 0

driver:	dd disk_ata_read
		dd disk_ata_to_errno

disk:	dd driver
		db 0
