/*
 * File: video_text_history.c
 * Author: Joel Souza
 * Date: 2026-03-30
 * Description: Functions to manage the video text history - A circular buffer that
 * 				overwrites the oldest content
 */

#include <stdint.h>
#include "drivers/video/text/video_text.h"
#include "drivers/video/text/config.h"
#include "string/string.h"
#include "circbuf/circbuf.h"

static inline
void	video_text_history_offset_advance_bounded(t_video_text *video_text, uint32_t history_max_bounded_ncells)
{
	t_video_text_history			* const history = &video_text->history;
	const t_video_text_config		* const config = video_text->config;
	const typeof(config->geometry)	* const geometry = &config->geometry;

	history->offset.cell = circbuf_offset_advance_bounded(history->offset.cell, history_max_bounded_ncells, VIDEO_TEXT_HISTORY_MAX);
	history->offset.column = circbuf_offset_advance(history->offset.column, history_max_bounded_ncells, geometry->ncolumns);
}

static inline
void	video_text_history_size_increment(t_video_text *video_text, uint32_t ncells)
{
	t_video_text_history			* const history = &video_text->history;
	const t_video_text_config		* const config = video_text->config;
	const typeof(config->geometry)	* const geometry = &config->geometry;
	const uint32_t					history_size_ncells = history->occupancy.ncells;

	if (history_size_ncells != VIDEO_TEXT_HISTORY_MAX)
	{
		history->occupancy.ncells = circbuf_size_increment(history_size_ncells, ncells, VIDEO_TEXT_HISTORY_MAX);
		history->occupancy.nlines = history->occupancy.ncells / geometry->ncolumns;
	}
}

// TODO: update doc
/*
 * NAME
 * 	video_text_history_write,
 * 	video_text_history_read,
 * 	video_text_history_set - Circular history buffer operations
 *
 * SYNOPSIS
 * 	typedef struct s_video_text_history
 * 	{
 * 		uint16_t	data[VIDEO_TEXT_HISTORY_MAX];
 * 		uint32_t	offset;
 * 		uint32_t	size;
 * 		uint32_t	nlines;
 * 		uint32_t	line_offset;
 * 	}	t_video_text_history;
 *
 * DESCRIPTION
 * 	video_text_history_write()
 * 		Writes up to 'count' bytes from 'text' into the history buffer,
 * 		interleaving each byte with 'attr', using a circular buffer layout.
 *
 * 		When the write exceeds VIDEO_TEXT_HISTORY_MAX, the oldest entries
 * 		are overwritten. If 'count' is greater than VIDEO_TEXT_HISTORY_MAX,
 * 		it is clamped to VIDEO_TEXT_HISTORY_MAX.
 *
 * 		The function updates the internal state of the history structure,
 * 		including offset, size, and line tracking fields.
 *
 * 	video_text_history_read()
 * 		Reads up to 'count' history entries into 'buf', beginning at the
 * 		position located 'rewind' entries before the current history offset
 * 		and traversing backwards.
 *
 * 		If 'rewind' is greater than or equal to history->size, no entries
 * 		are read.
 *
 * 		If 'count' exceeds the number of available entries
 * 		(history->size - rewind), the read is truncated.
 *
 * 	video_text_history_set()
 * 		Sets up to 'count' words to 'set' in the history buffer, beginning
 * 		at the position located 'rewind' entries before the current history
 * 		offset and traversing forward.
 *
 * 		If 'rewind' is greater than history->size, no entries are set.
 *
 * 		If 'count' is less than or equal to 'rewind', the write does not
 * 		reach the current history offset, and only existing history entries
 * 		are updated. The internal state of the history is unchanged.
 *
 * 		If 'count' is greater than 'rewind', the first 'rewind' entries
 * 		update the history up to the current history offset, and the
 * 		remaining entries are appended to the history. The internal state
 * 		of the history is updated accordingly.
 *
 * UNDEFINED BEHAVIOUR
 * 	Passing a history structure with invalid internal state.
 *
 * 	Examples include:
 * 		- history->offset >= VIDEO_TEXT_HISTORY_MAX
 * 		- history->size > VIDEO_TEXT_HISTORY_MAX
 * 		- history->nlines > VIDEO_TEXT_HISTORY_MAX / g_video_text_config.width
 * 		- history->line_offset >= g_video_text_config.width
 * 		- Metadata fields do not represent the current internal state
 * 		  of the history
 *
 * RETURN VALUE
 * 	video_text_history_write(), video_text_history_set()
 *
 * 		Returns the number of history entries written:
 * 			min(count, VIDEO_TEXT_HISTORY_MAX)
 *
 * 	video_text_history_read()
 *
 * 		Returns the number of history entries read:
 * 			min(count, history->size - rewind)
 *
 * 		If rewind >= history->size, returns 0.
 * */
uint32_t	video_text_history_write(t_video_text *video_text, const char *text, uint32_t count, uint8_t attr)
{
	t_video_text_history			* const history = &video_text->history;
	const t_video_text_config		* const config = video_text->config;
	const typeof(config->geometry)	* const geometry = &config->geometry;
	uint32_t						ncells_until_wrap_up;
	uint32_t						overwrite_tail_ncells;

	if (count > VIDEO_TEXT_HISTORY_MAX)
	{
		text += count - VIDEO_TEXT_HISTORY_MAX;
		overwrite_tail_ncells = count % VIDEO_TEXT_HISTORY_MAX;
		video_text_history_offset_advance_bounded(video_text, overwrite_tail_ncells);
		count = VIDEO_TEXT_HISTORY_MAX;
	}
	else
		history->offset.column = circbuf_offset_advance(history->offset.column, count, geometry->ncolumns);
	ncells_until_wrap_up = VIDEO_TEXT_HISTORY_MAX - history->offset.cell;
	if (count > ncells_until_wrap_up)
	{
		memicpy8(&history->data[history->offset.cell], text, ncells_until_wrap_up, attr);
		history->offset.cell = count - ncells_until_wrap_up;
		memicpy8(history->data, &text[ncells_until_wrap_up], history->offset.cell, attr);
	}
	else
	{
		memicpy8(&history->data[history->offset.cell], text, count, attr);
		history->offset.cell += count;
	}
	video_text_history_size_increment(video_text, count);
	return (count);
}

uint32_t	video_text_history_read(const t_video_text *video_text, uint32_t rewind, uint16_t *buf, uint32_t count)
{
	const t_video_text_history	* const history = &video_text->history;
	uint32_t					ncells_until_wrap_down;
	uint32_t					second_chunk_ncells;
	uint32_t					offset_cell;

	if (rewind >= history->occupancy.ncells)
		return (0);
	offset_cell = circbuf_offset_rewind_bounded(history->offset.cell, rewind + 1, VIDEO_TEXT_HISTORY_MAX);
	count = MIN(count, history->occupancy.ncells - rewind);
	ncells_until_wrap_down = offset_cell + 1;
	if (count > ncells_until_wrap_down)
	{
		second_chunk_ncells = count - ncells_until_wrap_down;
		memcpy(&buf[second_chunk_ncells], history->data, ncells_until_wrap_down * sizeof(uint16_t));
		offset_cell = VIDEO_TEXT_HISTORY_MAX - second_chunk_ncells;
		memcpy(buf, &history->data[offset_cell], second_chunk_ncells * sizeof(uint16_t));
	}
	else
	{
		offset_cell = ncells_until_wrap_down - count;
		memcpy(buf, &history->data[offset_cell], count * sizeof(uint16_t));
	}
	return (count);
}

uint32_t	video_text_history_set(t_video_text *video_text, uint32_t rewind, uint16_t set, uint32_t count)
{
	t_video_text_history			* const history = &video_text->history;
	const t_video_text_config		* const config = video_text->config;
	const typeof(config->geometry)	* const geometry = &config->geometry;
	uint32_t						ncells_until_wrap_up;
	uint32_t						ncells_appended;
	uint32_t						offset_cell;

	if (rewind > history->occupancy.ncells)
		return (0);
	if (count >= VIDEO_TEXT_HISTORY_MAX)
	{
		ncells_appended = count % VIDEO_TEXT_HISTORY_MAX;
		if (ncells_appended < rewind)
			ncells_appended += VIDEO_TEXT_HISTORY_MAX;
		ncells_appended -= rewind;
		video_text_history_offset_advance_bounded(video_text, ncells_appended);
		history->occupancy.ncells = VIDEO_TEXT_HISTORY_MAX;
		history->occupancy.nlines = config->history_max_lines;
		memsetw(history->data, set, VIDEO_TEXT_HISTORY_MAX);
		return (VIDEO_TEXT_HISTORY_MAX);
	}
	offset_cell = circbuf_offset_rewind_bounded(history->offset.cell, rewind, VIDEO_TEXT_HISTORY_MAX);
	ncells_until_wrap_up = VIDEO_TEXT_HISTORY_MAX - offset_cell;
	if (count > ncells_until_wrap_up)
	{
		memsetw(&history->data[offset_cell], set, ncells_until_wrap_up);
		offset_cell = count - ncells_until_wrap_up;
		memsetw(history->data, set, offset_cell);
	}
	else
	{
		memsetw(&history->data[offset_cell], set, count);
		offset_cell += count;
	}
	if (rewind >= count)
		return (count);
	ncells_appended = count - rewind;
	history->offset.cell = offset_cell;
	history->offset.column = circbuf_offset_advance(history->offset.column, ncells_appended, geometry->ncolumns);
	video_text_history_size_increment(video_text, ncells_appended);
	return (count);
}
