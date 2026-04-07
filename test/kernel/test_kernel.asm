; File: test_kernel.asm
; Author: Joel Souza
; Date: 2026-04-07
; Description: Tests kernel entry point (32-bit mode)

[BITS 32]

%include "test/kernel/test_kernel.inc"

section .asm
test_kernel_start:
	call	test_kernel_main
	jmp		$
