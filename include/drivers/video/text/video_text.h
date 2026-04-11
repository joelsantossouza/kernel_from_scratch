/*
 * File: video_text.h
 * Author: Joel Souza
 * Date: 2026-03-31
 * Description: Functions and constants definition of Video Text mode interface
 */

#ifndef VIDEO_TEXT_H
# define VIDEO_TEXT_H

# include <stdint.h>

// VGA
# define VIDEO_TEXT_VGA_ADDR	(uint16_t *)0xB8000

enum e_vga_text_color
{
	VGA_TEXT_BLACK = 0x0,
	VGA_TEXT_DARK_BLUE,
	VGA_TEXT_DARK_GREEN,
	VGA_TEXT_DARK_CYAN,
	VGA_TEXT_DARK_RED,
	VGA_TEXT_DARK_MAGENTA,
	VGA_TEXT_BROWN,
	VGA_TEXT_LIGHT_GRAY,
	VGA_TEXT_DARK_GRAY,
	VGA_TEXT_LIGHT_BLUE,
	VGA_TEXT_LIGHT_GREEN,
	VGA_TEXT_LIGHT_CYAN,
	VGA_TEXT_LIGHT_RED,
	VGA_TEXT_LIGHT_MAGENTA,
	VGA_TEXT_YELLOW,
	VGA_TEXT_WHITE
};

typedef struct s_video_text_config
{
	uint32_t	width;
	uint32_t	height;
	uint32_t	screensize;
	uint32_t	last_row;
}	t_video_text_config;

extern t_video_text_config	g_video_text_config;

// History
uint32_t	video_text_history_write(const char *text, uint32_t count, uint8_t attr);
uint32_t	video_text_history_read(uint32_t rewind, uint16_t *buf, uint32_t count);

// Operations
uint32_t	video_text_write(uint16_t *addr, const char *text, uint32_t count, uint8_t attr);

#endif
