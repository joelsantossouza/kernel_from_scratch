; File: mbr.inc
; Author: Joel Souza
; Date: 2026-03-11
; Description: MBR definitions and offsets layout

%ifndef MBR_INC
%define MBR_INC

%include "drivers/disk/vdl/config.inc"

%define MBR_ADDR					0x7C00
%define MBR_BYTES					VDL_SECTOR_BYTES

%define MBR_BOOT_CODE_BYTES			446

; Partition table
%define MBR_PART_TABLE_ENTRY_BYTES	16
%define MBR_PART_TYPE_FAT16_LBA		0x0E

%define MBR_PART0_LBA				2048
%define MBR_PART0_SECTORS			32768

%endif
