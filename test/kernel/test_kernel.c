/*
 * File: test_kernel.c
 * Author: Joel Souza
 * Date: 2026-04-07
 * Description: Tests kernel main
 */

#include "kernel/kernel.h"

#include "drivers/video/text/video_text.h"

char	getchar(void);

int	test_kernel_main(void)
{
	while (1)
	{
		char c = getchar();
		video_text_vga_write(&c, 1, VGA_TEXT_WHITE);
	}
	return (KERNEL_SUCCESS);
}

#include "drivers/io/io.h"
char g_scancode[] = \
    "\0\033123456789-=\010" \
    "\tqwertyuiop[]\r\0" \
    "asdfghjkl;'`\0\\zxcvbnm,./\0" \
    "*\0 \0\0\0\0\0\0\0\0\0\0\0" \
    "\0\0\0\0\0\0\0\0\0\0\0\0\0";

#include <stdint.h>
char	getchar(void)
{
	uint8_t	key_idx;

	while (1)
	{
		while ((io_inb(0x64) & 0x01) == 0)
			;
		key_idx = io_inb(0x60);
		if ((key_idx & 0x80) == 0)
			break ;
	}
	return (g_scancode[key_idx]);
}

