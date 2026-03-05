; File: io.inc
; Author: Joel Souza
; Date: 2026-03-05
; Description: I/O ports function declarations to Assembly usage

%ifndef IO_INC
%define IO_INC

extern	io_inb
extern	io_inw
extern	io_ind

extern	io_outb
extern	io_outw
extern	io_outd

%endif
