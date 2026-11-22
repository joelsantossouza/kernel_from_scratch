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

// TODO: Consider updates if final decision is to always write data on video text instead of just return in some cases
void	video_text_scroll_up(t_video_text *video_text, uint16_t *graphics_array_addr, uint32_t nlines)
{
	const t_video_text_history			* const history = &video_text->history;
	const t_video_text_config			* const config = video_text->config;
	const typeof(config->dimensions)	* const dimensions = &config->dimensions;
	uint32_t							columns_remaining;
	uint32_t							scroll_ncells;
	uint32_t							scroll_next;
	uint32_t							scroll_max;

	if (history->size.nlines < dimensions->lines)
		return ;
	scroll_max = history->size.nlines - dimensions->lines;
	scroll_next = video_text->scroll + nlines;
	if (scroll_next > scroll_max)
		scroll_next = scroll_max;
	if (video_text->scroll == scroll_next)
		return ;
	video_text->scroll = scroll_next;
	columns_remaining = dimensions->columns - history->offset.column;
	scroll_ncells = video_text->scroll * dimensions->columns - columns_remaining;
	video_text_history_read(video_text, scroll_ncells, graphics_array_addr, dimensions->cells);
}

// TODO: Consider updates if final decision is to always write data on video text instead of just return in some cases
void	video_text_scroll_down(t_video_text *video_text, uint16_t *graphics_array_addr, uint32_t nlines)
{
	const t_video_text_history			* const history = &video_text->history;
	const t_video_text_config			* const config = video_text->config;
	const typeof(config->dimensions)	* const dimensions = &config->dimensions;
	uint32_t							columns_remaining;
	uint32_t							padding_nbytes;
	uint32_t							scroll_ncells;
	uint32_t							scroll_next;

	if (video_text->scroll <= nlines)
		scroll_next = 0;
	else
		scroll_next = video_text->scroll - nlines;
	if (video_text->scroll == scroll_next)
		return ;
	columns_remaining = dimensions->columns - history->offset.column;
	video_text->scroll = scroll_next;
	if (video_text->scroll > 0)
	{
		scroll_ncells = video_text->scroll * dimensions->columns - columns_remaining;
		video_text_history_read(video_text, scroll_ncells, graphics_array_addr, dimensions->cells);
		return ;
	}
	video_text_history_read(video_text, 0, graphics_array_addr, video_text->screen_offset);
	padding_nbytes = columns_remaining * sizeof(uint16_t);
	memset(graphics_array_addr + video_text->screen_offset, 0, padding_nbytes);
}

// TODO: padding only remaing line or screensize cells?
void	video_text_scroll_to_bottom(t_video_text *video_text, uint16_t *graphics_array_addr)
{
	const t_video_text_config			* const config = video_text->config;
	const typeof(config->dimensions)	* const dimensions = &config->dimensions;
	const uint32_t						screen_remaining = dimensions->cells - video_text->screen_offset;
	const uint32_t						padding_nbytes = screen_remaining * sizeof(uint16_t);

	video_text->scroll = 0;
	video_text_history_read(video_text, 0, graphics_array_addr, video_text->screen_offset);
	memset(graphics_array_addr + video_text->screen_offset, 0, padding_nbytes);
}

