; File: config.asm
; Author: Joel Souza
; Date: 2026-03-01
; Description: Provide Bootloader configuration constants and macros

%define SECTOR_SIZE			512
%define BOOTABLE_SIGNATURE	0xaa55

%macro BYTES_WRITTEN 0
	($ - $$)
%endmacro
