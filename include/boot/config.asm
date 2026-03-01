; File: config.asm
; Author: Joel Souza
; Date: 2026-03-01
; Description: Provide Bootloader configuration constants and macros

%define SECTOR_SIZE			512
%define BYTES_WRITTEN		($ - $$)
%define BOOTABLE_SIGNATURE	0xaa55
