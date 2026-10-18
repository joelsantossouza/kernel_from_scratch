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
uint32_t	video_text_escape_newline(t_video_text *video_text, uint32_t count)
{
	const t_video_text_history	* const history = &video_text->history;
	const t_video_text_config	* const config = video_text->config;
	const uint32_t				set_cells = config->width * count - history->line_offset;

	return (video_text_history_set(video_text, 0, 0, set_cells));
}

static const t_video_text_escape_fn	g_video_text_escape_handlers[ASCII_TABLE_MAX] = {
	['\n'] = video_text_escape_newline,
};

//TODO: maybe improve last part of function to always scroll to bottom if data will ever be written on video text.
void	video_text_write(t_video_text *video_text, uint16_t *graphics_array_addr, const char *text, uint32_t count, uint8_t attr)
{
	const t_video_text_history	* const history = &video_text->history;
	const t_video_text_config	* const config = video_text->config;
	const uint32_t				old_screen_offset = video_text->screen_offset;
	const char					*text_end = text + count;
	const char					*text_ptr;
	uint32_t					entries_written;
	uint8_t						text_old_chr;

	if (text == text_end)
		return ;
	entries_written = 0;
	if (isprint((uint8_t)*text))
		goto video_text_write_printable;
	goto video_text_write_control;
	while (text < text_end)
	{
video_text_write_printable:
		text_ptr = text++;
		while (text < text_end && isprint((uint8_t)*text))
			text++;
		entries_written += video_text_history_write(video_text, text_ptr, text - text_ptr, attr);
		while (text < text_end && iscntrl((uint8_t)*text))
		{
video_text_write_control:
			text_old_chr = *text;
			text_ptr = text++;
			while (text < text_end && *text == text_old_chr)
				text++;
			entries_written += g_video_text_escape_handlers[text_old_chr](video_text, text - text_ptr);
		}
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
