; File: bios.inc
; Author: Joel Souza
; Date: 2026-03-04
; Description: BIOS commands, interrupts and constant macros

%ifndef BIOS_INC
%define BIOS_INC

; BIOS commands
%define BIOS_DRIVE_READ				0x02
%define BIOS_DRIVE_WRITE			0x03

%define BIOS_STR_WRITE				0x13

; BIOS text colors
%define BIOS_TEXT_WHITE				0x0F
%define BIOS_TEXT_RED				0x04

; BIOS interrupt
%define BIOS_INT_VIDEO				0x10
%define BIOS_INT_DRIVE				0x13

; BIOS signatures
%define BIOS_BOOT_LOAD_ADDR			0x7C00
%define BIOS_BOOT_SIGNATURE			0xAA55
%define BIOS_BOOT_SIGNATURE_BYTES	2

%endif
