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

uint32_t	video_text_history_write(uint32_t offset, const char *text, uint32_t count, uint8_t attr)
{
	uint32_t	space_until_wrap_up;

	if (offset >= VIDEO_TEXT_HISTORY_MAX)
		offset %= VIDEO_TEXT_HISTORY_MAX;
	if (count > VIDEO_TEXT_HISTORY_MAX)
	{
		text += count - VIDEO_TEXT_HISTORY_MAX;
		count = VIDEO_TEXT_HISTORY_MAX;
	}
	space_until_wrap_up = VIDEO_TEXT_HISTORY_MAX - offset;
	if (count > space_until_wrap_up)
	{
		memicpy8(&g_video_text_history[offset], text, space_until_wrap_up, attr);
		offset = count - space_until_wrap_up;
		memicpy8(g_video_text_history, &text[space_until_wrap_up], offset, attr);
	}
	else
	{
		memicpy8(&g_video_text_history[offset], text, count, attr);
		offset = count;
	}
	return (offset);
}

uint32_t	video_text_history_read(uint32_t offset, uint16_t *buf, uint32_t count)
{
	uint32_t	space_until_wrap_down;

	if (offset >= VIDEO_TEXT_HISTORY_MAX)
		offset %= VIDEO_TEXT_HISTORY_MAX;
	if (count > VIDEO_TEXT_HISTORY_MAX)
		count = VIDEO_TEXT_HISTORY_MAX;
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
	return (offset);
}
