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
#include "math/math.h"

t_video_text_history	g_video_text_history = {0};

/*
 * NAME
 * 	video_text_history_write,
 * 	video_text_history_read - Circular history buffer operations
 *
 * SYNOPSIS
 * 	typedef struct s_video_text_history
 * 	{
 * 		uint16_t	data[VIDEO_TEXT_HISTORY_MAX];
 * 		uint32_t	offset;
 * 		uint32_t	size;
 * 		uint32_t	lines;
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
 * 		If 'count' exceeds the number of available entries
 * 		(history->size - rewind), the read is truncated.
 *
 * 		If 'rewind' is greater than or equal to history->size, no entries
 * 		are read.
 *
 * UNDEFINED BEHAVIOUR
 * 	Passing a history structure with invalid internal state.
 *
 * 	Examples include:
 * 		- history->offset >= VIDEO_TEXT_HISTORY_MAX
 * 		- history->size > VIDEO_TEXT_HISTORY_MAX
 * 		- history->lines > VIDEO_TEXT_HISTORY_MAX / g_video_text_config.width
 * 		- history->line_offset >= g_video_text_config.width
 * 		- Metadata fields do not represent the current internal state
 * 		  of the history
 *
 * RETURN VALUE
 * 	video_text_history_write()
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
uint32_t	video_text_history_write(t_video_text_history *history, const char *text, uint32_t count, uint8_t attr)
{
	uint32_t	space_until_wrap_up;
	uint32_t	overwrite_tail;
	uint32_t	next_offset;

	if (count > VIDEO_TEXT_HISTORY_MAX)
	{
		text += count - VIDEO_TEXT_HISTORY_MAX;
		overwrite_tail = count % VIDEO_TEXT_HISTORY_MAX;
		next_offset = history->offset + overwrite_tail;
		history->offset = next_offset >= VIDEO_TEXT_HISTORY_MAX
			? next_offset - VIDEO_TEXT_HISTORY_MAX : next_offset;
		history->line_offset += overwrite_tail;
		count = VIDEO_TEXT_HISTORY_MAX;
	}
	space_until_wrap_up = VIDEO_TEXT_HISTORY_MAX - history->offset;
	if (count > space_until_wrap_up)
	{
		memicpy8(&history->data[history->offset], text, space_until_wrap_up, attr);
		history->offset = count - space_until_wrap_up;
		memicpy8(history->data, &text[space_until_wrap_up], history->offset, attr);
	}
	else
	{
		memicpy8(&history->data[history->offset], text, count, attr);
		history->offset += count;
	}
	history->line_offset += count;
	if (history->line_offset >= g_video_text_config.width)
		history->line_offset %= g_video_text_config.width;
	video_text_history_size_increment(history, count);
	return (count);
}

uint32_t	video_text_history_read(const t_video_text_history *history, uint32_t rewind, uint16_t *buf, uint32_t count)
{
	uint32_t	space_until_wrap_down;
	uint32_t	second_chunk_size;
	uint32_t	offset;

	if (rewind >= history->size)
		return (0);
	offset = (history->offset + VIDEO_TEXT_HISTORY_MAX - rewind - 1) % VIDEO_TEXT_HISTORY_MAX;
	count = MIN(count, history->size - rewind);
	space_until_wrap_down = offset + 1;
	if (count > space_until_wrap_down)
	{
		second_chunk_size = count - space_until_wrap_down;
		memcpy(&buf[second_chunk_size], history->data, space_until_wrap_down * sizeof(uint16_t));
		offset = VIDEO_TEXT_HISTORY_MAX - second_chunk_size;
		memcpy(buf, &history->data[offset], second_chunk_size * sizeof(uint16_t));
	}
	else
	{
		offset = space_until_wrap_down - count;
		memcpy(buf, &history->data[offset], count * sizeof(uint16_t));
	}
	return (count);
}

uint32_t	video_text_history_set(t_video_text_history *history, uint32_t rewind, uint16_t set, uint32_t count)
{
	uint32_t	space_until_wrap_up;
	uint32_t	offset;

	if (rewind > history->size)
		return (0);
	if (count > VIDEO_TEXT_HISTORY_MAX)
		count = VIDEO_TEXT_HISTORY_MAX;
	offset = (history->offset + VIDEO_TEXT_HISTORY_MAX - rewind) % VIDEO_TEXT_HISTORY_MAX;
	space_until_wrap_up = VIDEO_TEXT_HISTORY_MAX - offset;
	if (count > space_until_wrap_up)
	{
		memset(&history->data[offset], set, space_until_wrap_up * sizeof(uint16_t));
		offset = count - space_until_wrap_up;
		memset(history->data, set, offset * sizeof(uint16_t));
	}
	else
	{
		memset(&history->data[offset], set, count * sizeof(uint16_t));
		offset += count;
	}
	if (rewind >= count)
		return (count);
	history->offset = offset;
	history->line_offset += count;
	if (history->line_offset >= g_video_text_config.width)
		history->line_offset %= g_video_text_config.width;
	video_text_history_size_increment(history, count);
	return (count);
}
