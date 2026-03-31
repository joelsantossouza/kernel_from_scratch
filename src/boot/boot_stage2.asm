; File: boot_stage2.asm
; Author: Joel Souza
; Date: 2026-03-01
; Description: Once MBR has a limited size to boot code (only 446 bytes),
;			   the actual code to load kernel from filesystem will reside
;			   here in the sector 1 until sector 2047 (~1MB MBR gap)

[BITS 32]

%include "drivers/video/text/video_text.inc"

section	.text
boot_stage2:
	push	0x7f
	push	dword [text_size]
	push	text
	call	video_text_vga_write
	add		esp, 12

	jmp		$


section	.data
text:		db "This is a big text to test if the Video Text History read/write functions are working correctly. If you are being able to read its sentences without any obvious missing part, you passed in the test!!"
text_size:	dd $ - text
