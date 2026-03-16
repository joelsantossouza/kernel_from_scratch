; File: mbr.asm
; Author: Joel Souza
; Date: 2026-03-01
; Description: Master Boot Record. This is the first sector (512 bytes size) of
; 			   the hard disk. It will load the stage 2 of boot into RAM.

%include "boot/config.inc"
%include "boot/bios.inc"
%include "cpu/gdt.inc"
%include "cpu/cr0.inc"

%ifidn __OUTPUT_FORMAT__, bin
 [ORG MBR_ADDR]
%endif

jmp	short boot_start
nop

;;
; gdt - Global Descriptor Table
;
; DESCRIPTION
;	Initial setup of Global Descriptor Table to
;	enter in Protected Mode.
;;
gdt:
.null:
	dq	0x0

.kernel_code:
	dw	0xffff
	dw	0x0
	db	0x0
	db	10011010b
	db	11001111b
	db	0x0

.kernel_data:
	dw	0xffff
	dw	0x0
	db	0x0
	db	10010010b
	db	11001111b
	db	0x0
.end:

;;
; gdt_descriptor - GDT address information
;
; DESCRIPTION
;	Describes where in memory the GDT is stored
;	and its size. This 6 bytes structure will be
;	loaded to CPU with lgdt instruction
;;
gdt_descriptor:
	dw	gdt.end - gdt - 1
	dd	gdt

[BITS 16]
;;
; boot_panic - Panic on boot phase
;
; DESCRIPTION
;	Prints an error message on screen and enters an
;	infinite loop, preventing further execution
;
; RETURN VALUE
;	None
;;
BOOT_PANIC_STR:		db "Error: Could not boot system!"
BOOT_PANIC_STRLEN	equ $ - BOOT_PANIC_STR
boot_panic:
	mov	ah, BIOS_STR_WRITE
	mov	al, 0x0
	mov	bh, 0x0
	mov	bl, BIOS_TEXT_RED
	mov	cx, BOOT_PANIC_STRLEN
	mov	dh, 0x0
	mov	dl, 0x0
	mov	bp, BOOT_PANIC_STR
	int	BIOS_INT_VIDEO
	jmp	$

;;
; boot_start - Boot code
;
; DESCRIPTION
;	-> Save the booted drive number
;	-> Init segment registers (16-bit mode)
;	-> Load boot stage 2 in RAM
;	-> Loads GDT
;	-> Enter in Protected Mode
;	-> Init segment registers (32-bit mode)
;	-> Jump to boot stage 2 (Where will be loaded the kernel)
;
; RETURN VALUE
;	None
;;
BOOT_DRIVE_NO:	db 0
boot_start:
	mov	[BOOT_DRIVE_NO], dl	; Save boot drive number set by BIOS

.init_16bit_mode:
	mov	ax, 0x0
	mov	ds, ax
	mov	ss, ax
	mov	es, ax
	mov	fs, ax
	mov	gs, ax

.load_stage2:
	mov	ah, BIOS_DRIVE_READ
	mov	al, BOOT_STAGE2_SECTORS
	mov	ch, BOOT_STAGE2_CYLINDER
	mov	cl, BOOT_STAGE2_SECTOR
	mov	dh, BOOT_STAGE2_HEAD
	mov	dl, [BOOT_DRIVE_NO]
	mov	bx, BOOT_STAGE2_ADDR
	int	BIOS_INT_DRIVE

	jc	boot_panic
	cmp	al, BOOT_STAGE2_SECTORS
	jne	boot_panic

.enter_32bit_mode:
	cli
	lgdt	[gdt_descriptor]
	mov		eax, cr0
	or		eax, CPU_CR0_PE
	mov		cr0, eax
	jmp		GDT_KERNEL_CODE:.init_32bit_mode

[BITS 32]
.init_32bit_mode:
	mov	ax, GDT_KERNEL_DATA
	mov	ds, ax
	mov	ss, ax
	mov	es, ax
	mov	fs, ax
	mov	gs, ax
	mov	ebp, MBR_ADDR
	mov	esp, ebp

	jmp	BOOT_STAGE2_ADDR

times	MBR_CODE_BYTES - ($ - $$) db 0

partition_table:
.part0:
	db		0
	db		0, 0, 0
	db		MBR_PART_TYPE_FAT16_LBA
	db		0, 0, 0
	dd		MBR_PART0_LBA
	dd		MBR_PART0_SECTORS

.part1:
	times	MBR_PARTITION_TABLE_ENTRY_BYTES db 0

.part2:
	times	MBR_PARTITION_TABLE_ENTRY_BYTES db 0

.part3:
	times	MBR_PARTITION_TABLE_ENTRY_BYTES db 0

dw	BOOT_SIGNATURE
