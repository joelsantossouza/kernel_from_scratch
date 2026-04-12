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

static uint16_t	g_video_text_history[VIDEO_TEXT_HISTORY_MAX];
static uint32_t	g_video_text_history_offset = 0;
static uint32_t	g_video_text_history_size = 0;
uint32_t		g_video_text_history_lines = 0;

uint32_t	video_text_history_write(const char *text, uint32_t count, uint8_t attr)
{
	uint32_t	space_until_wrap_up;

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
	if (g_video_text_history_size < VIDEO_TEXT_HISTORY_MAX)
	{
		g_video_text_history_size = MIN(g_video_text_history_size + count, VIDEO_TEXT_HISTORY_MAX);
		g_video_text_history_lines = ALIGN_UP(g_video_text_history_size, g_video_text_config.width) / g_video_text_config.width;
	}
	return (count);
}

uint32_t	video_text_history_read(uint32_t rewind, uint16_t *buf, uint32_t count)
{
	uint32_t	space_until_wrap_down;
	uint32_t	second_chunk_size;
	uint32_t	offset;

	if (rewind >= g_video_text_history_size)
		return (0);
	offset = (g_video_text_history_offset + VIDEO_TEXT_HISTORY_MAX - rewind - 1) % VIDEO_TEXT_HISTORY_MAX;
	count = MIN(count, g_video_text_history_size - rewind);
	space_until_wrap_down = offset + 1;
	if (count > space_until_wrap_down)
	{
		second_chunk_size = count - space_until_wrap_down;
		memcpy(&buf[second_chunk_size], g_video_text_history, space_until_wrap_down * sizeof(uint16_t));
		offset = VIDEO_TEXT_HISTORY_MAX - second_chunk_size;
		memcpy(buf, &g_video_text_history[offset], second_chunk_size * sizeof(uint16_t));
	}
	else
	{
		offset = space_until_wrap_down - count;
		memcpy(buf, &g_video_text_history[offset], count * sizeof(uint16_t));
	}
	return (count);
}

uint32_t	video_text_history_set(uint32_t rewind, uint16_t set, uint32_t count)
{

}
