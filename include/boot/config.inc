; File: config.inc
; Author: Joel Souza
; Date: 2026-03-01
; Description: Provide Bootloader configuration constants

%ifndef BOOT_CONFIG_INC
%define BOOT_CONFIG_INC

%include "drivers/disk/vdl/config.inc"

%define BOOT_LOAD_ADDR			0x7C00

%assign BOOT_STAGE2_ADDR		BOOT_LOAD_ADDR + VDL_SECTOR_BYTES
%define BOOT_STAGE2_SECTORS		1
%define BOOT_STAGE2_SECTOR		2
%define BOOT_STAGE2_CYLINDER	0
%define BOOT_STAGE2_HEAD		0

%define BOOT_SIGNATURE_SIZE		2
%define BOOT_SIGNATURE			0xAA55

%endif
