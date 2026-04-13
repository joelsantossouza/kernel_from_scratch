/*
 * File: test_kernel.c
 * Author: Joel Souza
 * Date: 2026-04-07
 * Description: Tests kernel main
 */

#include "kernel/kernel.h"
#include "string/string.h"
#include "drivers/video/text/video_text.h"
#include "drivers/io/io.h"

char g_scancode[] =
    "\0\0331234567890-=\010"
    "\tqwertyuiop[]\n\0"
    "asdfghjkl;'`\0\\zxcvbnm,./\0"
    "*\0 \0\0\0\0\0\0\0\0\0\0\0"
    "\0\0\0\0\0\0\0\0\0\0\0\0\0";

char g_scancode_shift[] =
    "\0\033!@#$%^&*()_+\010"
    "\tQWERTYUIOP{}\n\0"
    "ASDFGHJKL:\"~\0|ZXCVBNM<>?\0"
    "*\0 \0\0\0\0\0\0\0\0\0\0\0"
    "\0\0\0\0\0\0\0\0\0\0\0\0\0";

/* ---------------- PS/2 helpers ---------------- */

static void ps2_wait_input(void)
{
    /* wait until input buffer is empty (bit 1 == 0) */
    while (io_inb(0x64) & 0x02)
        ;
}

static void ps2_wait_output(void)
{
    /* wait until output buffer is full (bit 0 == 1) */
    while (!(io_inb(0x64) & 0x01))
        ;
}

/* ---------------- Keyboard init ---------------- */

void keyboard_init(void)
{
    /* Enable scanning on the keyboard:
     * 1) tell controller next byte is for keyboard (0xD4 on 0x64)
     * 2) send 0xF4 on 0x60
     */

    ps2_wait_input();
    io_outb(0x64, 0xD4);

    ps2_wait_input();
    io_outb(0x60, 0xF4);

    /* opcional: ler ACK (0xFA) se quiser */
    ps2_wait_output();
    (void)io_inb(0x60);
}

/* ---------------- getchar() ---------------- */

char getchar(void)
{
    uint8_t sc;
    static int shift = 0;

    for (;;)
    {
        ps2_wait_output();
        sc = io_inb(0x60);

        /* Shift press */
        if (sc == 0x2A || sc == 0x36) {
            shift = 1;
            continue;
        }

        /* Shift release */
        if (sc == 0xAA || sc == 0xB6) {
            shift = 0;
            continue;
        }

        /* ignore break codes */
        if (sc & 0x80)
            continue;

        char c = shift ? g_scancode_shift[sc] : g_scancode[sc];

        if (c != 0)
            return c;
    }
}

/* ---------------- Test kernel main ---------------- */

int test_kernel_main(void)
{
    keyboard_init();

	video_text_write(VIDEO_TEXT_VGA_ADDR, "1111122222333334444455555666667\n7778888899999aaaaabbbbbcccccdddddeeeeefffffggggghhhhhiiiii", 80, VGA_TEXT_WHITE);
	video_text_write(VIDEO_TEXT_VGA_ADDR, "11111222223333344444555556\n6666777778888899999aaaaabbbbbcccccdddddeeeeefffffggggghhhhhiiiii", 80, VGA_TEXT_WHITE);
	video_text_write(VIDEO_TEXT_VGA_ADDR, "111112222233333444445555566666777778888899999aaaaabbbbbcccccdddddeeeeefffffggggghhhhhiiiii", 80, VGA_TEXT_WHITE);
	video_text_write(VIDEO_TEXT_VGA_ADDR, "111112222233333444445555566666777778888899999aaaaabbbbbcccccdddddeeeeefffffggggghhhhhiiiii", 80, VGA_TEXT_WHITE);
	video_text_write(VIDEO_TEXT_VGA_ADDR, "111112222233333444445555566666777778888899999aaaaabbbbbcccccdddddeeeeefffffggggghhhhhiiiii", 80, VGA_TEXT_WHITE);
	video_text_write(VIDEO_TEXT_VGA_ADDR, "111112222233333444445555566666777778888899999aaaaabbbbbcccccdddddeeeeefffffggggghhhhhiiiii", 80, VGA_TEXT_WHITE);
	video_text_write(VIDEO_TEXT_VGA_ADDR, "111112222233333444445555566666777778888899999aaaaabbbbbcccccdddddeeeeefffffggggghhhhhiiiii", 80, VGA_TEXT_WHITE);
	video_text_write(VIDEO_TEXT_VGA_ADDR, "111112222233333444445555566666777778888899999aaaaabbbbbcccccdddddeeeeefffffggggghhhhhiiiii", 80, VGA_TEXT_WHITE);
	video_text_write(VIDEO_TEXT_VGA_ADDR, "111112222233333444445555566666777778888899999aaaaabbbbbcccccdddddeeeeefffffggggghhhhhiiiii", 80, VGA_TEXT_WHITE);
	video_text_write(VIDEO_TEXT_VGA_ADDR, "111112222233333444445555566666777778888899999aaaaabbbbbcccccdddddeeeeefffffggggghhhhhiiiii", 80, VGA_TEXT_WHITE);
	video_text_write(VIDEO_TEXT_VGA_ADDR, "111112222233333444445555566666777778888899999aaaaabbbbbcccccdddddeeeeefffffggggghhhhhiiiii", 80, VGA_TEXT_WHITE);
	video_text_write(VIDEO_TEXT_VGA_ADDR, "111112222233333444445555566666777778888899999aaaaabbbbbcccccdddddeeeeefffffggggghhhhhiiiii", 80, VGA_TEXT_WHITE);
	video_text_write(VIDEO_TEXT_VGA_ADDR, "111112222233333444445555566666777778888899999aaaaabbbbbcccccdddddeeeeefffffggggghhhhhiiiii", 80, VGA_TEXT_WHITE);
	video_text_write(VIDEO_TEXT_VGA_ADDR, "111112222233333444445555566666777778888899999aaaaabbbbbcccccdddddeeeeefffffggggghhhhhiiiii", 80, VGA_TEXT_WHITE);
	video_text_write(VIDEO_TEXT_VGA_ADDR, "111112222233333444445555566666777778888899999aaaaabbbbbcccccdddddeeeeefffffggggghhhhhiiiii", 80, VGA_TEXT_WHITE);
	video_text_write(VIDEO_TEXT_VGA_ADDR, "111112222233333444445555566666777778888899999aaaaabbbbbcccccdddddeeeeefffffggggghhhhhiiiii", 80, VGA_TEXT_WHITE);
	video_text_write(VIDEO_TEXT_VGA_ADDR, "111112222233333444445555566666777778888899999aaaaabbbbbcccccdddddeeeeefffffggggghhhhhiiiii", 80, VGA_TEXT_WHITE);
	video_text_write(VIDEO_TEXT_VGA_ADDR, "111112222233333444445555566666777778888899999aaaaabbbbbcccccdddddeeeeefffffggggghhhhhiiiii", 80, VGA_TEXT_WHITE);

    while (1)
    {
        char c = getchar();
		if (c == 'k')
			video_text_scroll_up(VIDEO_TEXT_VGA_ADDR, 1);
		else if (c == 'j')
			video_text_scroll_down(VIDEO_TEXT_VGA_ADDR, 1);
		else
        	video_text_write(VIDEO_TEXT_VGA_ADDR, &c, 1, VGA_TEXT_WHITE);
    }

    return KERNEL_SUCCESS;
}
