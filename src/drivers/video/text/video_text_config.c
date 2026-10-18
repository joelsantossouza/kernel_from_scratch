/*
 * File: video_text_config.c
 * Author: Joel Souza
 * Date: 2026-04-11
 * Description: Video Text Controllers common configurations - Shared configurations
 * 				between text rendering controllers such as screensize, width and height.
 */

#include "drivers/video/text/video_text.h"
#include "drivers/video/text/config.h"

t_video_text_config	g_video_text_config_default = {
	.width = VIDEO_TEXT_WIDTH_DEFAULT,
	.height = VIDEO_TEXT_HEIGHT_DEFAULT,
	.screensize = VIDEO_TEXT_WIDTH_DEFAULT * VIDEO_TEXT_HEIGHT_DEFAULT,
	.last_row = VIDEO_TEXT_WIDTH_DEFAULT * VIDEO_TEXT_HEIGHT_DEFAULT - VIDEO_TEXT_WIDTH_DEFAULT,
};
