; File: mbr.inc
; Author: Joel Souza
; Date: 2026-03-11
; Description: MBR definitions and offsets layout

%ifndef MBR_INC
%define MBR_INC

%include "boot/bios.inc"
%include "drivers/disk/disk.inc"

%define MBR_ADDR					BIOS_BOOT_LOAD_ADDR
%define MBR_BYTES					DISK_SECTOR_BYTES

; Partition table
%define MBR_PART_TABLE_ENTRY_BYTES	16
%define MBR_PART_TABLE_ENTRY_COUNT	4
%assign MBR_PART_TABLE_BYTES		MBR_PART_TABLE_ENTRY_COUNT * MBR_PART_TABLE_ENTRY_BYTES
%define MBR_PART_TYPE_FAT16_LBA		0x0E

; Boot code
%assign MBR_BOOT_CODE_BYTES			MBR_BYTES - MBR_PART_TABLE_BYTES - BIOS_BOOT_SIGNATURE_BYTES

%endif
