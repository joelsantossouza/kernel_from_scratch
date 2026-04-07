; File: disk.inc
; Author: Joel Souza
; Date: 2026-04-07
; Description: Disk geometry constants and arithmetic macros

%ifndef DISK_INC
%define DISK_INC

%define DISK_SECTOR_BYTES	512
%assign DISK_SECTOR_WORDS	DISK_SECTOR_BYTES / 2

%endif
