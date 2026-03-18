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
	; Mounts
	push	mount_path1
	push	0
	push	disk
	call	vfs_mount
	add		esp, 12

	push	mount_path2
	push	0
	push	disk
	call	vfs_mount
	add		esp, 12

	push	mount_path3
	push	0
	push	disk
	call	vfs_mount
	add		esp, 12

	; Search mount
	push	path
	call	vfs_mount_find
	add		esp, 4
	jmp	$

section	.data
path:			db "/kernel/bootable/home/joesanto/exercises/file.txt", 0
mount_path1:	db "/kernel/boot", 0
mount_path2:	db "/", 0
mount_path3:	db "/kernel", 0

driver:
	dd	disk_ata_read
	dd	disk_ata_to_errno

disk:
	dd	driver
	db	0
