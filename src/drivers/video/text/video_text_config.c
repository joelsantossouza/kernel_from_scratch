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
	.dimensions = {
		.columns = VIDEO_TEXT_DIMENSIONS_COLUMNS_DEFAULT,
		.lines = VIDEO_TEXT_DIMENSIONS_LINES_DEFAULT,
		.cells = VIDEO_TEXT_DIMENSIONS_COLUMNS_DEFAULT * VIDEO_TEXT_DIMENSIONS_LINES_DEFAULT,
	},
	.last_line = VIDEO_TEXT_DIMENSIONS_COLUMNS_DEFAULT * VIDEO_TEXT_DIMENSIONS_LINES_DEFAULT - VIDEO_TEXT_DIMENSIONS_COLUMNS_DEFAULT,
	.history_max_lines = VIDEO_TEXT_HISTORY_MAX / VIDEO_TEXT_DIMENSIONS_COLUMNS_DEFAULT
};
