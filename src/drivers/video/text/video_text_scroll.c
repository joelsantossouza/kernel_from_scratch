/*
 * File: video_text_scroll.c
 * Author: Joel Souza
 * Date: 2026-04-11
 * Description: Text history scroll back implementation for Video Text rendering mode.
 * 				It uses the Video Text History API to manipulate the content displayed
 * 				on screen.
 */

#include <stdint.h>

static uint32_t	g_video_text_scroll = 0;

void	video_text_scrollup(uint32_t nlines)
{
	g_video_text_vga_scroll += nlines;
	video_text_history_read(g_video_text_vga_scroll, VIDEO_TEXT_VGA_ADDR, g_video_text_vga_config.screensize);
}

void	video_text_scrolldown(uint32_t nlines)
{
	const uint32_t	cells_to_scroll = nlines * g_video_text_vga_config.width;

	if (g_video_text_vga_scroll < cells_to_scroll)
		return ;
	g_video_text_vga_scroll -= cells_to_scroll;
	video_text_history_read(g_video_text_vga_scroll, VIDEO_TEXT_VGA_ADDR, g_video_text_vga_config.screensize);
}
