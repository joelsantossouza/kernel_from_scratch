/*
 * File: video_text.c
 * Author: Joel Souza
 * Date: 2026-03-30
 * Description: Functions to manage the video text - The video text structure is
 * 				a primitive representation of a terminal
 */

#include "drivers/video/text/video_text.h"
#include "string/string.h"
#include "ctype/ctype.h"

t_video_text g_video_text_default = {
	.history = {{0}},
	.config = &g_video_text_config_default,
	.screen_offset = 0,
	.scroll = 0
};

static
uint32_t	video_text_escape_newline(t_video_text *video_text)
{
	const t_video_text_history	* const history = &video_text->history;
	const t_video_text_config	* const config = video_text->config;
	const uint32_t				line_remaining = config->width - history->line_offset;

	return (video_text_history_set(video_text, 0, 0, line_remaining));
}

static const t_video_text_escape_fn	g_video_text_escape_handlers[ASCII_TABLE_MAX] = {
	['\n'] = video_text_escape_newline,
};

//TODO: maybe improve last part of function to always scroll to bottom if data will ever be written on video text.
//TODO: Before loop decide start loop (printable/control characters loop).
//TODO: In printable loop accumulate the number of printable character to print once
//TODO: In control loop accumulate the number of consecutive control character to transmit the paratmer N respectivelly
void	video_text_write(t_video_text *video_text, uint16_t *graphics_array_addr, const char *text, uint32_t count, uint8_t attr)
{
	const t_video_text_history	* const history = &video_text->history;
	const t_video_text_config	* const config = video_text->config;
	const uint32_t				old_screen_offset = video_text->screen_offset;
	const char					*text_end = text + count;
	const char					*text_ptr;
	uint32_t					entries_written;

	entries_written = 0;
	while (text < text_end)
	{
		text_ptr = text;
		while (text < text_end && isprint(*text))
			text++;
		if (text > text_ptr)
			entries_written += video_text_history_write(video_text, text_ptr, text - text_ptr, attr);
		while (text < text_end && iscntrl((uint8_t)*text))
			entries_written += g_video_text_escape_handlers[(uint8_t)*text++](video_text);
	}
	video_text->screen_offset += entries_written;
	if (video_text->screen_offset >= config->screensize)
	{
		video_text->screen_offset = config->last_row + history->line_offset;
		video_text_scroll_to_bottom(video_text, graphics_array_addr);
		return ;
	}
	if (video_text->scroll == 0)
		video_text_history_read(video_text, 0, graphics_array_addr + old_screen_offset, entries_written);
	else
		video_text_scroll_to_bottom(video_text, graphics_array_addr);
}
