/*
 * File: video_text_config.c
 * Author: Joel Souza
 * Date: 2026-04-11
 * Description: Video Text Controllers common configurations - Shared configurations
 * 				between text rendering controllers such as screensize, width and height.
 */

#include "drivers/video/text/video_text.h"
#include "drivers/video/text/config.h"

t_video_text_config	g_video_text_config = {
	.width = VIDEO_TEXT_DEFAULT_WIDTH,
	.height = VIDEO_TEXT_DEFAULT_HEIGHT,
	.screensize = VIDEO_TEXT_DEFAULT_WIDTH * VIDEO_TEXT_DEFAULT_HEIGHT,
	.last_row = VIDEO_TEXT_DEFAULT_WIDTH * VIDEO_TEXT_DEFAULT_HEIGHT - VIDEO_TEXT_DEFAULT_WIDTH,
};
