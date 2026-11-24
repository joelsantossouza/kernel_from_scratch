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
	.geometry.ncolumns = VIDEO_TEXT_GEOMETRY_COLUMNS_DEFAULT,
	.geometry.nlines = VIDEO_TEXT_GEOMETRY_LINES_DEFAULT,
	.geometry.ncells = VIDEO_TEXT_GEOMETRY_COLUMNS_DEFAULT * VIDEO_TEXT_GEOMETRY_LINES_DEFAULT,

	.geometry.last_line.column = 0,
	.geometry.last_line.line= VIDEO_TEXT_GEOMETRY_LINES_DEFAULT - 1,
	.geometry.last_line.cell = VIDEO_TEXT_GEOMETRY_COLUMNS_DEFAULT * VIDEO_TEXT_GEOMETRY_LINES_DEFAULT - VIDEO_TEXT_GEOMETRY_COLUMNS_DEFAULT,

	.history_max_lines = VIDEO_TEXT_HISTORY_MAX / VIDEO_TEXT_GEOMETRY_COLUMNS_DEFAULT
};
