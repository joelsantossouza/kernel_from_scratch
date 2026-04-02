; File: stddef.inc
; Author: Joel Souza
; Date: 2026-03-17
; Description: Assembly standard constant sizes and masks

%ifndef STDDEF_H
%define STDDEF_H

; Sizes
%define BYTE			1
%define WORD			2
%define DWORD			4
%define QWORD			8

; Masks
%assign BYTE_MASK		BYTE - 1
%assign WORD_MASK		WORD - 1
%assign DWORD_MASK		DWORD - 1
%assign QWORD_MASK		QWORD - 1

; Log2
%define BYTE_LOG2		0
%define WORD_LOG2		1
%define DWORD_LOG2		2
%define QWORD_LOG2		3

%endif
