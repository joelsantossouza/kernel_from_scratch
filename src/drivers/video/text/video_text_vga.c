/*
 * File: video_text_vga.c
 * Author: Joel Souza
 * Date: 2026-03-30
 * Description: Functions to load the video text history into VGA text buffer
 * 				and manage configurations of VGA text mode
 */

#include "drivers/video/text/video_text.h"
#include "drivers/video/text/config.h"
#include "math/math.h"

static t_video_text_config	g_video_text_vga_config = {
	.width = VIDEO_TEXT_DEFAULT_WIDTH,
	.height = VIDEO_TEXT_DEFAULT_HEIGHT,
	.screensize = VIDEO_TEXT_DEFAULT_WIDTH * VIDEO_TEXT_DEFAULT_HEIGHT
};
static uint32_t				g_video_text_vga_scroll = 0;

uint32_t	video_text_vga_write(const char *text, uint32_t count, enum e_vga_text_color color)
{
	uint32_t	history_entries_written;
	uint32_t	vga_entries_to_display;

	history_entries_written = video_text_history_write(text, count, color);
	vga_entries_to_display = MIN(history_entries_written, g_video_text_vga_config.screensize);
	g_video_text_vga_scroll = 0;
	video_text_history_read(0, VIDEO_TEXT_VGA_ADDR, vga_entries_to_display);
	return (vga_entries_to_display);
}
