/*
 * File: video_text_scroll.c
 * Author: Joel Souza
 * Date: 2026-04-11
 * Description: Text history scroll back implementation for Video Text rendering mode.
 * 				It uses the Video Text History API to manipulate the content displayed
 * 				on screen.
 */

#include <stdint.h>
#include "drivers/video/text/video_text.h"
#include "string/string.h"

uint32_t	g_video_text_scroll = 0;

void	video_text_scroll_up(uint16_t *video_text_addr, uint32_t nlines)
{
	uint32_t	scroll_max;
	uint32_t	scroll_next;
	uint32_t	scroll_cells;
	uint32_t	line_remaining;

	if (g_video_text_history_lines < g_video_text_config.height)
		return ;
	scroll_max = g_video_text_history_lines - g_video_text_config.height;
	scroll_next = g_video_text_scroll + nlines;
	if (scroll_next > scroll_max)
		scroll_next = scroll_max;
	if (g_video_text_scroll == scroll_next)
		return ;
	g_video_text_scroll = scroll_next;
	if (g_video_text_history_line_offset != 0)
		line_remaining = g_video_text_config.width - g_video_text_history_line_offset;
	else
		line_remaining = 0;
	scroll_cells = g_video_text_scroll * g_video_text_config.width - line_remaining;
	video_text_history_read(scroll_cells, video_text_addr, g_video_text_config.screensize);
}

void	video_text_scroll_down(uint16_t *video_text_addr, uint32_t nlines)
{
	uint32_t	scroll_next;
	uint32_t	scroll_cells;
	uint32_t	line_remaining;
	uint32_t	padding_bytes;

	if (g_video_text_scroll <= nlines)
		scroll_next = 0;
	else
		scroll_next = g_video_text_scroll - nlines;
	if (g_video_text_scroll == scroll_next)
		return ;
	if (g_video_text_history_line_offset != 0)
		line_remaining = g_video_text_config.width - g_video_text_history_line_offset;
	else
		line_remaining = 0;
	g_video_text_scroll = scroll_next;
	if (g_video_text_scroll > 0)
	{
		scroll_cells = g_video_text_scroll * g_video_text_config.width - line_remaining;
		video_text_history_read(scroll_cells, video_text_addr, g_video_text_config.screensize);
		return ;
	}
	video_text_history_read(0, video_text_addr, g_video_text_offset);
	padding_bytes = line_remaining * sizeof(uint16_t);
	memset(video_text_addr + g_video_text_offset, 0, padding_bytes);
}
