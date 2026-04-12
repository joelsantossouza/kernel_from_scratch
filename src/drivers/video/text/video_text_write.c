/*
 * File: video_text_write.c
 * Author: Joel Souza
 * Date: 2026-03-30
 * Description: Functions to load the Video Text History content
 * 				into Controller's text buffer
 */

#include "drivers/video/text/video_text.h"
#include "string/string.h"
#include "ctype/ctype.h"

uint32_t	g_video_text_offset = 0;
uint32_t	g_video_text_lineoffset = 0;

static
uint32_t	video_text_escape_newline(void)
{
	const uint32_t	line_remaining = g_video_text_config.width - g_video_text_lineoffset;

	return (video_text_history_set(0, 0, line_remaining));
}

static const t_video_text_escape_fn	g_video_text_escape_handlers[ASCII_TABLE_MAX] = {
	['\n'] = video_text_escape_newline,
};

void	video_text_write(uint16_t *video_text_addr, const char *text, uint32_t count, uint8_t attr)
{
	const uint32_t	curr_offset = g_video_text_offset;
	const char		*text_end = text + count;
	const char		*text_ptr;
	uint32_t		entries_written;

	entries_written = 0;
	while (text < text_end)
	{
		text_ptr = text;
		while (text < text_end && isprint(*text))
			text++;
		if (text > text_ptr)
			entries_written += video_text_history_write(text_ptr, text - text_ptr, attr);
		while (text < text_end && iscntrl(*text))
			entries_written += g_video_text_escape_handlers[*text++]();
	}
	g_video_text_lineoffset += entries_written;
	if (g_video_text_lineoffset >= g_video_text_config.width)
		g_video_text_lineoffset %= g_video_text_config.width;
	g_video_text_offset += entries_written;
	if (g_video_text_offset > g_video_text_config.screensize)
	{
		g_video_text_offset = g_video_text_config.last_row + g_video_text_lineoffset;
		video_text_scroll_to_bottom(video_text_addr);
		return ;
	}
	if (g_video_text_scroll == 0)
		video_text_history_read(0, video_text_addr + curr_offset, entries_written);
	else
		video_text_scroll_to_bottom(video_text_addr);
}
