; File: mbr.asm
; Author: Joel Souza
; Date: 2026-03-01
; Description: Master Boot Record. This is the first sector (512 bytes size) of
; 			   the hard disk. It will load the stage 2 of boot into RAM.

%include "drivers/disk/vdl/config.inc"
%include "cpu/gdt.inc"
%include "cpu/cr0.inc"
%include "boot/config.inc"

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
	db	10011010b
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

;;
; boot_start - Boot code
;
; DESCRIPTION
;	-> Load boot stage 2 in RAM
;	-> Loads GDT
;	-> Enter in Protected Mode
;	-> Init segment registers
;	-> Jump to boot stage 2 (Where will be loaded the kernel)
;
; RETURN VALUE
;	None
;;
boot_start:
[BITS 16]
.load_stage2:
; TODO: Write load of stage2

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

	jmp	boot_stage2

times	VDL_SECTOR_BYTES - ($ - $$) - BOOT_SIGNATURE_SIZE db 0
dw		BOOT_SIGNATURE

boot_stage2:
