/*
 * File: video_text.h
 * Author: Joel Souza
 * Date: 2026-03-31
 * Description: Functions and constants definition of Video Text mode interface
 */

#ifndef VIDEO_TEXT_H
# define VIDEO_TEXT_H

# include <stdint.h>
# include "drivers/video/text/config.h"
# include "string/string.h"
# include "math/math.h"

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

typedef uint32_t	(*t_video_text_escape_fn)(void);

extern t_video_text_config	g_video_text_config;
extern uint32_t				g_video_text_offset;
extern uint32_t				g_video_text_history_size;
extern uint32_t				g_video_text_history_lines;
extern uint32_t				g_video_text_history_line_offset;
extern uint32_t				g_video_text_scroll;

// History
uint32_t	video_text_history_write(const char *text, uint32_t count, uint8_t attr);
uint32_t	video_text_history_read(uint32_t rewind, uint16_t *buf, uint32_t count);
uint32_t	video_text_history_set(uint32_t rewind, uint16_t set, uint32_t count);

static inline
void	video_text_history_size_update(uint32_t new_size)
{
	if (g_video_text_history_size < VIDEO_TEXT_HISTORY_MAX)
	{
		g_video_text_history_size = MIN(new_size, VIDEO_TEXT_HISTORY_MAX);
		g_video_text_history_lines = ALIGN_UP(g_video_text_history_size, g_video_text_config.width) / g_video_text_config.width;
	}
}

static inline
void	video_text_history_size_increment(uint32_t amount)
{
	if (g_video_text_history_size < VIDEO_TEXT_HISTORY_MAX)
	{
		g_video_text_history_size = MIN(g_video_text_history_size + amount, VIDEO_TEXT_HISTORY_MAX);
		g_video_text_history_lines = ALIGN_UP(g_video_text_history_size, g_video_text_config.width) / g_video_text_config.width;
	}
}

// Scroll
void		video_text_scroll_up(uint16_t *video_text_addr, uint32_t nlines);
void		video_text_scroll_down(uint16_t *video_text_addr, uint32_t nlines);

static inline
void	video_text_scroll_to_bottom(uint16_t *video_text_addr)
{
	const uint32_t	screen_remaining = g_video_text_config.screensize - g_video_text_offset;
	const uint32_t	padding_bytes = screen_remaining * sizeof(uint16_t);

	g_video_text_scroll = 0;
	video_text_history_read(0, video_text_addr, g_video_text_offset);
	memset(video_text_addr + g_video_text_offset, 0, padding_bytes);
}

// Operations
void		video_text_write(uint16_t *video_text_addr, const char *text, uint32_t count, uint8_t attr);

#endif
