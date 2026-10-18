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

typedef struct s_video_text_config
{
	uint32_t					width;
	uint32_t					height;
	uint32_t					screensize;
	uint32_t					last_row;
}	t_video_text_config;

typedef struct s_video_text_history
{
	uint16_t					data[VIDEO_TEXT_HISTORY_MAX];
	uint32_t					offset;
	uint32_t					size;
	uint32_t					nlines;
	uint32_t					line_offset;
}	t_video_text_history;

typedef struct s_video_text
{
	t_video_text_history		history;
	const t_video_text_config	*config;
	uint32_t					screen_offset;
	uint32_t					scroll;
}	t_video_text;

typedef uint32_t	(*t_video_text_escape_fn)(t_video_text *video_text, uint32_t count);

extern t_video_text_config	g_video_text_config_default;
extern t_video_text			g_video_text_default;

uint32_t	video_text_history_write(t_video_text *video_text, const char *text, uint32_t count, uint8_t attr);
uint32_t	video_text_history_read(const t_video_text *video_text, uint32_t rewind, uint16_t *buf, uint32_t count);
uint32_t	video_text_history_set(t_video_text *video_text, uint32_t rewind, uint16_t set, uint32_t count);

void		video_text_scroll_up(t_video_text *video_text, uint16_t *graphics_array_addr, uint32_t nlines);
void		video_text_scroll_down(t_video_text *video_text, uint16_t *graphics_array_addr, uint32_t nlines);
void		video_text_scroll_to_bottom(t_video_text *video_text, uint16_t *graphics_array_addr);

void		video_text_write(t_video_text *video_text, uint16_t *graphics_array_addr, const char *text, uint32_t count, uint8_t attr);

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
	video_text_write(&g_video_text_default, VGA_TEXT_ADDR, text, count, color);
}

static inline
void	vga_text_print(const char *text, enum e_vga_text_color color)
{
	video_text_write(&g_video_text_default, VGA_TEXT_ADDR, text, strlen(text), color);
}

static inline
void	vga_text_scroll_up(uint32_t nlines)
{
	video_text_scroll_up(&g_video_text_default, VGA_TEXT_ADDR, nlines);
}

static inline
void	vga_text_scroll_down(uint32_t nlines)
{
	video_text_scroll_down(&g_video_text_default, VGA_TEXT_ADDR, nlines);
}

#endif
