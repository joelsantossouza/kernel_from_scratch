; File: boot_stage2.asm
; Author: Joel Souza
; Date: 2026-03-01
; Description: Once MBR has a limited size to boot code (only 446 bytes),
;			   the actual code to load kernel from filesystem will reside
;			   here in the sector 1 until sector 2047 (~1MB MBR gap)

[BITS 32]

%include "drivers/disk/vdl/vdl.inc"
%include "fs/vfs/vfs_partition.inc"
%include "fs/fat/fat.inc"
%include "cpu/gdt.inc"
%include "autoconfig.inc"

section	.text
boot_stage2:
	push	part0
	push	0
	push	g_disk0
	call	vfs_partition_init
	add		esp, 12

	push	kernel_file
	push	kernel_path
	push	part0
	call	fat_file_open
	add		esp, 12

	mov		eax, dword [kernel_file + 36]
	mov		dword [kernel_file_cluster], eax

	push	dword [kernel_file + 40]
	push	CONFIG_KERNEL_ADDR
	push	kernel_file_offset
	push	kernel_file_cluster
	push	part0
	call	fat_cluster_read
	add		esp, 20

	jmp		GDT_KERNEL_CODE:CONFIG_KERNEL_ADDR

section	.data
part0:
	times 1024 db 0 ;WARNING: It will change when malloc be implemented on FAT metadata

kernel_path:
	db "boot/kernel", 0

kernel_file:
	times 44 db 0

kernel_file_cluster:
	dd 0

kernel_file_offset:
	dd 0
