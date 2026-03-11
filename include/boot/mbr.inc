; File: mbr.inc
; Author: Joel Souza
; Date: 2026-03-11
; Description: MBR definitions and offsets layout

%ifndef MBR_INC
%define MBR_INC

%include "drivers/disk/vdl/config.inc"

%define MBR_ADDR	0x7C00
%define MBR_BYTES	VDL_SECTOR_BYTES

%endif
