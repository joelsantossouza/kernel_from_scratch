; File: boot_stage2.asm
; Author: Joel Souza
; Date: 2026-03-01
; Description: Once MBR has a limited size to boot code (only 446 bytes),
;			   the actual code to load kernel from filesystem will reside
;			   here in the sector 1 until sector 2047 (~1MB MBR gap)

[BITS 32]

%include "drivers/disk/ata/ata.inc"
%include "fs/vfs/vfs_mount_table.inc"
%include "fs/vfs/vfs_partition.inc"

section	.text
boot_stage2:
	; Init partition
	push	part1
	push	0
	push	disk
	call	vfs_partition_init
	add		esp, 12

	; Mounts
	push	dword [mount_pathlen1]
	push	mount_path1
	push	part1
	call	vfs_mount_table_insert
	add		esp, 12

	push	dword [mount_pathlen2]
	push	mount_path2
	push	part1
	call	vfs_mount_table_insert
	add		esp, 12

	push	dword [mount_pathlen3]
	push	mount_path3
	push	part1
	call	vfs_mount_table_insert
	add		esp, 12

	; Search mount
	push	dword [path]
	call	vfs_mount_table_find
	add		esp, 4
	jmp	$

section	.data
path:			db "/kernel/bootable/home/joesanto/exercises/file.txt", 0
pathlen:		dd $ - path

mount_path1:	db "/kernel/boot"
mount_pathlen1:	dd $ - mount_path1

mount_path2:	db "/"
mount_pathlen2:	dd $ - mount_path2

mount_path3:	db "/kernel/took"
mount_pathlen3:	dd $ - mount_path3

driver:
	dd	disk_ata_read
	dd	disk_ata_to_errno

disk:
	dd	driver
	db	0

part1: times 8 db 0
