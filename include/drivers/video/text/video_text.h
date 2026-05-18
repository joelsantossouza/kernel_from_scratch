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

typedef uint32_t	(*t_video_text_escape_fn)(void);

typedef struct s_video_text_config
{
	uint32_t	width;
	uint32_t	height;
	uint32_t	screensize;
	uint32_t	last_row;
}	t_video_text_config;

extern t_video_text_config	g_video_text_config;
extern uint32_t				g_video_text_offset;

// History
typedef struct s_video_text_history
{
	uint16_t	data[VIDEO_TEXT_HISTORY_MAX];
	uint32_t	offset;
	uint32_t	size;
	uint32_t	lines;
	uint32_t	line_offset;
}	t_video_text_history;

extern t_video_text_history	g_video_text_history;

uint32_t	video_text_history_write(t_video_text_history *history, const char *text, uint32_t count, uint8_t attr);
uint32_t	video_text_history_read(const t_video_text_history *history, uint32_t rewind, uint16_t *buf, uint32_t count);
uint32_t	video_text_history_set(t_video_text_history *history, uint32_t rewind, uint16_t set, uint32_t count);

static inline
void	video_text_history_size_update(t_video_text_history *history, uint32_t new_size)
{
	if (history->size < VIDEO_TEXT_HISTORY_MAX)
	{
		history->size = MIN(new_size, VIDEO_TEXT_HISTORY_MAX);
		history->lines = align_up(history->size, g_video_text_config.width) / g_video_text_config.width;
	}
}

static inline
void	video_text_history_size_increment(t_video_text_history *history, uint32_t amount)
{
	if (history->size < VIDEO_TEXT_HISTORY_MAX)
	{
		history->size = MIN(history->size + amount, VIDEO_TEXT_HISTORY_MAX);
		history->lines = align_up(history->size, g_video_text_config.width) / g_video_text_config.width;
	}
}

// Scroll
extern uint32_t	g_video_text_scroll;

void		video_text_scroll_up(uint16_t *video_text_addr, uint32_t nlines);
void		video_text_scroll_down(uint16_t *video_text_addr, uint32_t nlines);

static inline
void	video_text_scroll_to_bottom(uint16_t *video_text_addr)
{
	const uint32_t	screen_remaining = g_video_text_config.screensize - g_video_text_offset;
	const uint32_t	padding_bytes = screen_remaining * sizeof(uint16_t);

	g_video_text_scroll = 0;
	video_text_history_read(&g_video_text_history, 0, video_text_addr, g_video_text_offset);
	memset(video_text_addr + g_video_text_offset, 0, padding_bytes);
}

// Operations
void		video_text_write(uint16_t *video_text_addr, const char *text, uint32_t count, uint8_t attr);

// VGA
# define VGA_TEXT_ADDR	(uint16_t *)0xB8000

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

static inline
void	vga_text_write(const char *text, uint32_t count, enum e_vga_text_color color)
{
	video_text_write(VGA_TEXT_ADDR, text, count, color);
}

static inline
void	vga_text_print(const char *text, enum e_vga_text_color color)
{
	video_text_write(VGA_TEXT_ADDR, text, strlen(text), color);
}

static inline
void	vga_text_scroll_up(uint32_t nlines)
{
	video_text_scroll_up(VGA_TEXT_ADDR, nlines);
}

static inline
void	vga_text_scroll_down(uint32_t nlines)
{
	video_text_scroll_down(VGA_TEXT_ADDR, nlines);
}

#endif
