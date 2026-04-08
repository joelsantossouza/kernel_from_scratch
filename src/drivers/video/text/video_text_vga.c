/*
 * File: video_text_vga.c
 * Author: Joel Souza
 * Date: 2026-03-30
 * Description: Functions to load the video text history into VGA text buffer
 * 				and manage configurations of VGA text mode
 */

#include "drivers/video/text/video_text.h"
#include "drivers/video/text/config.h"
#include "math/math.h"

static t_video_text_config	g_video_text_vga_config = {
	.width = VIDEO_TEXT_DEFAULT_WIDTH,
	.height = VIDEO_TEXT_DEFAULT_HEIGHT,
	.screensize = VIDEO_TEXT_DEFAULT_WIDTH * VIDEO_TEXT_DEFAULT_HEIGHT
};
static uint32_t				g_video_text_vga_scroll = 0;
static uint32_t				g_video_text_vga_cursor = 0;

uint32_t	video_text_vga_write(const char *text, uint32_t count, enum e_vga_text_color color)
{
	uint32_t	entries_written;
	uint32_t	cursor_next;
	uint32_t	last_line;

	entries_written = video_text_history_write(text, count, color);
	cursor_next = g_video_text_vga_cursor + entries_written;
	if (cursor_next > g_video_text_vga_config.screensize)
	{
		last_line = g_video_text_vga_config.screensize - g_video_text_vga_config.width;
		g_video_text_vga_cursor = last_line + cursor_next % g_video_text_vga_config.width;
		video_text_history_read(0, VIDEO_TEXT_VGA_ADDR, g_video_text_vga_cursor);
		// WARNING: Use memset
		cursor_next %= g_video_text_vga_config.width;
		while (cursor_next < g_video_text_vga_config.width)
			*(VIDEO_TEXT_VGA_ADDR + last_line + cursor_next++) = 0;
		// WARNING: Use memset
		return (g_video_text_vga_cursor);
	}
	video_text_history_read(0, VIDEO_TEXT_VGA_ADDR + g_video_text_vga_cursor, entries_written);
	g_video_text_vga_cursor += entries_written;
	return (entries_written);
}

void	video_text_vga_scrollup(uint32_t nlines)
{
	g_video_text_vga_scroll += nlines * g_video_text_vga_config.width;
	video_text_history_read(g_video_text_vga_scroll, VIDEO_TEXT_VGA_ADDR, g_video_text_vga_config.screensize);
}

void	video_text_vga_scrolldown(uint32_t nlines)
{
	const uint32_t	cells_to_scroll = nlines * g_video_text_vga_config.width;

	if (g_video_text_vga_scroll < cells_to_scroll)
		return ;
	g_video_text_vga_scroll -= cells_to_scroll;
	video_text_history_read(g_video_text_vga_scroll, VIDEO_TEXT_VGA_ADDR, g_video_text_vga_config.screensize);
}
