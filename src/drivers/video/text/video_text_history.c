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

uint16_t	g_video_text_history[VIDEO_TEXT_HISTORY_MAX];

uint32_t	video_text_history_write(uint32_t offset, const char *text, uint32_t size, uint8_t attr)
{
	uint32_t	remaining_until_limit;

	if (offset >= VIDEO_TEXT_HISTORY_MAX)
		offset %= VIDEO_TEXT_HISTORY_MAX;
	if (size > VIDEO_TEXT_HISTORY_MAX)
	{
		text += VIDEO_TEXT_HISTORY_MAX - size;
		size = VIDEO_TEXT_HISTORY_MAX;
	}
	remaining_until_limit = VIDEO_TEXT_HISTORY_MAX - offset;
	if (size > remaining_until_limit)
	{
		memicpy8(&g_video_text_history[offset], text, remaining_until_limit, attr);
		offset = size - remaining_until_limit;
		memicpy8(g_video_text_history, &text[remaining_until_limit], offset, attr);
	}
	else
	{
		memicpy8(&g_video_text_history[offset], text, size, attr);
		offset = size;
	}
	return (offset);
}

uint32_t	video_text_history_read(uint32_t offset, uint16_t *buf, uint32_t size)
{
	if (offset >= VIDEO_TEXT_HISTORY_MAX)
		offset %= VIDEO_TEXT_HISTORY_MAX;
	if (size > VIDEO_TEXT_HISTORY_MAX)
		size = VIDEO_TEXT_HISTORY_MAX;
	if (size > offset)
	{
		size -= offset;
		memcpy(&buf[size], g_video_text_history, offset);
		offset = VIDEO_TEXT_HISTORY_MAX - size;
		memcpy(buf, &g_video_text_history[offset], size);
	}
	else
	{
		offset -= size;
		memcpy(buf, &g_video_text_history[offset], size);
	}
	return (offset);
}
