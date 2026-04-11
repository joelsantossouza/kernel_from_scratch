/*
 * File: video_text_write.c
 * Author: Joel Souza
 * Date: 2026-03-30
 * Description: Functions to load the Video Text History content
 * 				into Controller's text buffer
 */

#include "drivers/video/text/video_text.h"
#include "string/string.h"
#include "math/math.h"

uint32_t	g_video_text_offset = 0;
uint32_t	g_video_text_lineoffset = 0;

uint32_t	video_text_write(uint16_t *video_text_addr, const char *text, uint32_t count, uint8_t attr)
{
	const uint32_t	entries_written = video_text_history_write(text, count, attr);
	const uint32_t	curr_offset = g_video_text_offset;
	uint32_t		line_remaining;
	uint32_t		padding_bytes;

	g_video_text_lineoffset += entries_written;
	if (g_video_text_lineoffset >= g_video_text_config.width)
		g_video_text_lineoffset %= g_video_text_config.width;
	g_video_text_offset += entries_written;
	if (g_video_text_offset <= g_video_text_config.screensize)
	{
		video_text_history_read(0, video_text_addr + curr_offset, entries_written);
		return (entries_written);
	}
	g_video_text_offset = g_video_text_config.last_row + g_video_text_lineoffset;
	video_text_history_read(0, video_text_addr, g_video_text_offset);
	line_remaining = g_video_text_config.width - g_video_text_lineoffset;
	padding_bytes = line_remaining * sizeof(uint16_t);
	memset(video_text_addr + g_video_text_offset, 0, padding_bytes);
	return (MIN(entries_written, g_video_text_offset));
}
