; File: mbr.asm
; Author: Joel Souza
; Date: 2026-03-01
; Description: Master Boot Record. This is the first sector (512 bytes size) of
; 			   the hard disk. This will be a bootable section that will load the
;			   kernel into RAM

%include "boot/config.asm"

[BITS 16]

section	.mbr
_start:
	mov	ah, 0x0e
	mov	al, 'a'
	mov	dx, 0
	int 0x10

times	SECTOR_SIZE - BYTES_WRITTEN - 2 db 0
dw		BOOTABLE_SIGNATURE
