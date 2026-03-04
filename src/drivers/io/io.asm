; File: io.asm
; Author: Joel Souza
; Date: 2026-03-02
; Description: Drivers to read and write through x86 I/O ports

[BITS 32]

section	.asm
;;
; io_inb, io_inw, io_ind - Read byte/word/double word from I/O port
;
; uint8_t	io_inb(uint16_t port);
; uint16_t	io_inw(uint16_t port);
; uint32_t	io_ind(uint16_t port);
;
; DESCRIPTION
;	io_inb, io_inw and io_ind read one byte, one word
;	and one double word from a specific x86 I/O port respectively.
;
; RETURN VALUE
;	Return the byte/word/double word read from the port
;;
io_inb:
	push	ebp
	mov		ebp, esp

	mov		edx, [ebp + 8]
	in		al, dx

	pop		ebp
	ret

io_inw:
	push	ebp
	mov		ebp, esp

	mov		edx, [ebp + 8]
	in		ax, dx

	pop		ebp
	ret

io_ind:
	push	ebp
	mov		ebp, esp

	mov		edx, [ebp + 8]
	in		eax, dx

	pop		ebp
	ret

;;
; io_outb, io_outw, io_outd - Write byte/word/double word to I/O port
;
; void	io_outb(uint16_t port, uint8_t value);
; void	io_outw(uint16_t port, uint16_t value);
; void	io_outd(uint16_t port, uint32_t value);
;
; DESCRIPTION
;	io_outb, io_outw and io_outd write a byte, a word and a
;	double word value in a specific x86 I/O port respectively.
;
; RETURN VALUE
;	None
;;
io_outb:
	push	ebp
	mov		ebp, esp

	mov		edx, [ebp + 8]
	mov		eax, [ebp + 12]
	out		dx, al

	pop		ebp
	ret

io_outw:
	push	ebp
	mov		ebp, esp

	mov		edx, [ebp + 8]
	mov		eax, [ebp + 12]
	out		dx, ax

	pop		ebp
	ret

io_outd:
	push	ebp
	mov		ebp, esp

	mov		edx, [ebp + 8]
	mov		eax, [ebp + 12]
	out		dx, eax

	pop		ebp
	ret
