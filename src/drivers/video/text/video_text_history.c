/*
 * File: video_text_history.c
 * Author: Joel Souza
 * Date: 2026-03-30
 * Description: Functions to manage the video text history - A circular buffer that
 * 				overwrites the oldest content
 */

#include <stdint.h>
#include "drivers/video/text/config.h"
#include "string/string.h"

static uint16_t	g_video_text_history[VIDEO_TEXT_HISTORY_MAX];
static uint32_t	g_video_text_history_offset = 0;

uint32_t	video_text_history_write(const char *text, uint32_t count, uint8_t attr)
{
	uint32_t	space_until_wrap_up;

	if (g_video_text_history_offset >= VIDEO_TEXT_HISTORY_MAX)
		g_video_text_history_offset %= VIDEO_TEXT_HISTORY_MAX;
	if (count > VIDEO_TEXT_HISTORY_MAX)
	{
		text += count - VIDEO_TEXT_HISTORY_MAX;
		count = VIDEO_TEXT_HISTORY_MAX;
	}
	space_until_wrap_up = VIDEO_TEXT_HISTORY_MAX - g_video_text_history_offset;
	if (count > space_until_wrap_up)
	{
		memicpy8(&g_video_text_history[g_video_text_history_offset], text, space_until_wrap_up, attr);
		g_video_text_history_offset = count - space_until_wrap_up;
		memicpy8(g_video_text_history, &text[space_until_wrap_up], g_video_text_history_offset, attr);
	}
	else
	{
		memicpy8(&g_video_text_history[g_video_text_history_offset], text, count, attr);
		g_video_text_history_offset += count;
	}
	return (count);
}

uint32_t	video_text_history_read(uint32_t rewind, uint16_t *buf, uint32_t count)
{
	uint32_t	space_until_wrap_down;
	uint32_t	max_to_read;
	uint32_t	entries_to_read;
	uint32_t	offset;

	if (rewind >= VIDEO_TEXT_HISTORY_MAX)
		return (0);
	max_to_read = VIDEO_TEXT_HISTORY_MAX - rewind;
	offset = (g_video_text_history_offset + VIDEO_TEXT_HISTORY_MAX - rewind) % VIDEO_TEXT_HISTORY_MAX;
	if (count > max_to_read)
		count = max_to_read;
	entries_to_read = count;
	space_until_wrap_down = offset + 1;
	if (count > space_until_wrap_down)
	{
		count -= space_until_wrap_down;
		memcpy(&buf[count], g_video_text_history, space_until_wrap_down * sizeof(uint16_t));
		offset = VIDEO_TEXT_HISTORY_MAX - count;
		memcpy(buf, &g_video_text_history[offset], count * sizeof(uint16_t));
	}
	else
	{
		offset = space_until_wrap_down - count;
		memcpy(buf, &g_video_text_history[offset], count * sizeof(uint16_t));
	}
	return (entries_to_read);
}
