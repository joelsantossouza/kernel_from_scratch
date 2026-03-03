; File: config.inc
; Author: Joel Souza
; Date: 2026-03-01
; Description: Virtual Disk Layer (VDL) configuration constants for hard disk management

%ifndef VDL_CONFIG_INC
%define VDL_CONFIG_INC

%define VDL_SECTOR_BYTES	512
%assign VDL_SECTOR_WORDS	VDL_SECTOR_BYTES / 2

%endif
