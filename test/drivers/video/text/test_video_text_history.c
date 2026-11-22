/*
 * File: test_video_text_history.c
 * Author: Joel Souza
 * Date: 2026-07-30
 * Description: Test video text history management functions
 */

#include "drivers/video/text/config.h"
#include "drivers/video/text/video_text.h"
#include "test/unit_test/unit_test.h"
#include "test/drivers/video/text/test_video_text_history.h"
#include "circbuf/circbuf.h"

UT_CREATE_CATEGORY(video_text_history, "Test Video Text History functionalities")

static char					g_src_raw[SRC_SIZE];
static uint16_t				g_src_attributed[SRC_SIZE];
static uint16_t				g_src_uniform_words[SRC_SIZE];
static uint16_t				g_result[VIDEO_TEXT_HISTORY_MAX];

const uint32_t				VIDEO_TEXT_HISTORY_1_PERCENT = VIDEO_TEXT_HISTORY_MAX * 0.01;

__attribute__((constructor))
static void	src_init(void)
{
	memset_pattern(g_src_raw, SRC_PATTERN, strlen(SRC_PATTERN), SRC_SIZE);
	memicpy8(g_src_attributed, g_src_raw, SRC_SIZE, SRC_ATTR);
	memsetw(g_src_uniform_words, SRC_UNIFORM_WORD, SRC_SIZE);
}

static t_video_text	g_video_text_test = {
	.history = {{0}},
	.config = &g_video_text_config_default,
	.screen_offset = 0,
	.scroll = 0
};
static t_video_text_history						*g_history_test_ptr = &g_video_text_test.history;
static const t_video_text_config				*g_config_ptr = &g_video_text_config_default;
static const typeof(g_config_ptr->dimensions)	*g_dimensions_ptr = &g_video_text_config_default.dimensions;

static
void	history_test_init(t_video_text_history *history_test, uint32_t offset_cell, uint32_t size_ncells)
{
	history_test->offset.cell = offset_cell;
	history_test->offset.column = offset_cell % g_dimensions_ptr->columns;
	history_test->size.ncells = size_ncells;
	history_test->size.nlines = size_ncells / g_dimensions_ptr->columns;
	memset(history_test->data, 0, VIDEO_TEXT_HISTORY_MAX * sizeof(uint16_t));
	memset(g_result, 0, VIDEO_TEXT_HISTORY_MAX * sizeof(uint16_t));
}

static t_video_text_history						g_history_expect;
static uint32_t									g_ret;

static
void	history_expect_side_effects_append(t_video_text_history *history_expect, const t_video_text_history *history_test, uint32_t ncells)
{
	history_expect->offset.cell = circbuf_offset_advance(history_test->offset.cell, ncells, VIDEO_TEXT_HISTORY_MAX);
	history_expect->offset.column = history_expect->offset.cell % g_dimensions_ptr->columns;
	history_expect->size.ncells = circbuf_size_increment(history_test->size.ncells, ncells, VIDEO_TEXT_HISTORY_MAX);
	history_expect->size.nlines = history_expect->size.ncells / g_dimensions_ptr->columns;
}

static
void	history_expect_side_effects_compare(const t_video_text_history *history_expect, const t_video_text_history *history_test)
{
	UT_EXPECT_EQ(history_expect->offset.cell, history_test->offset.cell);
	UT_EXPECT_EQ(history_expect->offset.column, history_test->offset.column);
	UT_EXPECT_EQ(history_expect->size.ncells, history_test->size.ncells);
	UT_EXPECT_EQ(history_expect->size.nlines, history_test->size.nlines);
}

/*
 * uint32_t	video_text_history_write(t_video_text_history *history, const char *text, uint32_t count, uint8_t attr);
 *
 * Writes the first 'count' bytes of 'text' byte-interleaved with 'attr' into 'history->data',
 * updating the history state properties such as offset, size, etc...
 * */
UT_CREATE_SUITE(video_text_history, write, "Test history write functionality")
UT_CREATE_CASE(video_text_history, write, no_wrap, "Writing data without history wrap")
{
	const uint32_t	ncells = VIDEO_TEXT_HISTORY_1_PERCENT;

	history_test_init(g_history_test_ptr, 0, 0);
	history_expect_side_effects_append(&g_history_expect, g_history_test_ptr, ncells);

	UT_LOG_CALL(g_ret = video_text_history_write(&g_video_text_test, g_src_raw, ncells, SRC_ATTR));

	UT_EXPECT_EQMEM(g_src_attributed, g_history_test_ptr->data, ncells * sizeof(uint16_t));
	UT_EXPECT_EQ(ncells, g_ret);
	history_expect_side_effects_compare(&g_history_expect, g_history_test_ptr);
}
UT_CREATE_CASE(video_text_history, write, with_wrap, "Writing data with history wrap")
{
	const uint32_t	total_ncells = VIDEO_TEXT_HISTORY_1_PERCENT;
	const uint32_t	chunk1_ncells = total_ncells / 2;
	const uint32_t	chunk2_ncells = total_ncells - chunk1_ncells;
	const uint32_t	initial_offset_cell = VIDEO_TEXT_HISTORY_MAX - chunk1_ncells;

	history_test_init(g_history_test_ptr, initial_offset_cell, 0);
	history_expect_side_effects_append(&g_history_expect, g_history_test_ptr, total_ncells);

	UT_LOG_CALL(g_ret = video_text_history_write(&g_video_text_test, g_src_raw, total_ncells, SRC_ATTR));
	UT_LOG_CALL(memcpy(g_result, &g_history_test_ptr->data[initial_offset_cell], chunk1_ncells * sizeof(uint16_t)));
	UT_LOG_CALL(memcpy(&g_result[chunk1_ncells], g_history_test_ptr->data, chunk2_ncells * sizeof(uint16_t)));

	UT_EXPECT_EQMEM(g_src_attributed, g_result, total_ncells * sizeof(uint16_t));
	UT_EXPECT_EQ(total_ncells, g_ret);
	history_expect_side_effects_compare(&g_history_expect, g_history_test_ptr);
}
UT_CREATE_CASE(video_text_history, write, over_history_max, "Writing more than VIDEO_TEXT_HISTORY_MAX cells")
{
	const uint32_t	overwrite_ncells = 1;
	const uint32_t	oversized_ncells = VIDEO_TEXT_HISTORY_MAX + overwrite_ncells;
	const uint32_t	remaining_ncells = VIDEO_TEXT_HISTORY_MAX - overwrite_ncells;

	history_test_init(g_history_test_ptr, 0, 0);
	history_expect_side_effects_append(&g_history_expect, g_history_test_ptr, oversized_ncells);

	UT_LOG_CALL(g_ret = video_text_history_write(&g_video_text_test, g_src_raw, oversized_ncells, SRC_ATTR));
	UT_LOG_CALL(memcpy(g_result, &g_history_test_ptr->data[overwrite_ncells], remaining_ncells * sizeof(uint16_t)));
	UT_LOG_CALL(memcpy(&g_result[remaining_ncells], g_history_test_ptr->data, overwrite_ncells * sizeof(uint16_t)));

	UT_EXPECT_EQMEM(&g_src_attributed[overwrite_ncells], g_result, VIDEO_TEXT_HISTORY_MAX * sizeof(uint16_t));
	UT_EXPECT_EQ(VIDEO_TEXT_HISTORY_MAX, g_ret);
	history_expect_side_effects_compare(&g_history_expect, g_history_test_ptr);
}


/*
 * uint32_t	video_text_history_read(const t_video_text_history *history, uint32_t rewind, uint16_t *buf, uint32_t count)
 *
 * Reads 'count' entries from 'history', starting at (history->offset - rewind) and traversing backwards.
 * The read entries are stored into 'buf'.
 * */
UT_CREATE_SUITE(video_text_history, read, "Test history read functionality")
UT_CREATE_CASE(video_text_history, read, no_wrap, "Reading data without history wrap")
{
	const uint32_t	ncells = VIDEO_TEXT_HISTORY_1_PERCENT;
	const uint32_t	offset_cell = ncells % VIDEO_TEXT_HISTORY_MAX;

	history_test_init(g_history_test_ptr, offset_cell, ncells);
	UT_LOG_CALL(memcpy(g_history_test_ptr->data, g_src_attributed, ncells * sizeof(uint16_t)));
	UT_LOG_CALL(g_ret = video_text_history_read(&g_video_text_test, 0, g_result, ncells));
	UT_EXPECT_EQMEM(g_src_attributed, g_result, ncells * sizeof(uint16_t));
	UT_EXPECT_EQ(ncells, g_ret);
}
UT_CREATE_CASE(video_text_history, read, with_wrap, "Reading data with history wrap")
{
	const uint32_t	total_ncells = VIDEO_TEXT_HISTORY_1_PERCENT;
	const uint32_t	chunk1_ncells = total_ncells / 2;
	const uint32_t	chunk2_ncells = total_ncells - chunk1_ncells;
	const uint32_t	initial_offset_cell = VIDEO_TEXT_HISTORY_MAX - chunk1_ncells;

	history_test_init(g_history_test_ptr, chunk2_ncells, total_ncells);
	UT_LOG_CALL(memcpy(&g_history_test_ptr->data[initial_offset_cell], g_src_attributed, chunk1_ncells * sizeof(uint16_t)));
	UT_LOG_CALL(memcpy(g_history_test_ptr->data, &g_src_attributed[chunk1_ncells], chunk2_ncells * sizeof(uint16_t)));
	UT_LOG_CALL(g_ret = video_text_history_read(&g_video_text_test, 0, g_result, total_ncells));
	UT_EXPECT_EQMEM(g_src_attributed, g_result, total_ncells * sizeof(uint16_t));
	UT_EXPECT_EQ(total_ncells, g_ret);
}
UT_CREATE_CASE(video_text_history, read, over_history_size, "Reading more than the current history ncells")
{
	const uint32_t	history_ncells = VIDEO_TEXT_HISTORY_1_PERCENT;
	const uint32_t	offset_cell = history_ncells % VIDEO_TEXT_HISTORY_MAX;
	const uint32_t	extra_ncells = 1;
	const uint32_t	oversized_ncells = history_ncells + extra_ncells;

	history_test_init(g_history_test_ptr, offset_cell, history_ncells);
	memcpy(g_history_expect.data, g_src_attributed, history_ncells * sizeof(uint16_t));
	memcpy(&g_history_expect.data[history_ncells], &g_result[history_ncells], extra_ncells * sizeof(uint16_t));
	UT_LOG_CALL(memcpy(g_history_test_ptr->data, g_src_attributed, history_ncells * sizeof(uint16_t)));
	UT_LOG_CALL(g_ret = video_text_history_read(&g_video_text_test, 0, g_result, oversized_ncells));
	UT_EXPECT_EQMEM(g_history_expect.data, g_result, oversized_ncells * sizeof(uint16_t));
	UT_EXPECT_EQ(history_ncells, g_ret);
}
UT_CREATE_CASE(video_text_history, read, in_bounds_rewind, "Reading data with rewind < current history ncells")
{
	const uint32_t	history_ncells = VIDEO_TEXT_HISTORY_1_PERCENT;
	const uint32_t	offset_cell = history_ncells % VIDEO_TEXT_HISTORY_MAX;
	const uint32_t	remaining_ncells = 1;
	const uint32_t	rewind_ncells = history_ncells - remaining_ncells;

	history_test_init(g_history_test_ptr, offset_cell, history_ncells);
	memcpy(g_history_expect.data, g_src_attributed, remaining_ncells * sizeof(uint16_t));
	memcpy(&g_history_expect.data[remaining_ncells], &g_result[remaining_ncells], rewind_ncells * sizeof(uint16_t));
	UT_LOG_CALL(memcpy(g_history_test_ptr->data, g_src_attributed, history_ncells * sizeof(uint16_t)));
	UT_LOG_CALL(g_ret = video_text_history_read(&g_video_text_test, rewind_ncells, g_result, history_ncells));
	UT_EXPECT_EQMEM(g_history_expect.data, g_result, history_ncells * sizeof(uint16_t));
	UT_EXPECT_EQ(remaining_ncells, g_ret);
}
UT_CREATE_CASE(video_text_history, read, out_of_bounds_rewind, "Reading data with rewind >= current history ncells")
{
	const uint32_t	history_ncells = VIDEO_TEXT_HISTORY_1_PERCENT;
	const uint32_t	offset_cell = history_ncells % VIDEO_TEXT_HISTORY_MAX;
	const uint32_t	rewind_ncells = history_ncells;

	history_test_init(g_history_test_ptr, offset_cell, history_ncells);
	memcpy(g_history_expect.data, g_result, history_ncells * sizeof(uint16_t));
	UT_LOG_CALL(memcpy(g_history_test_ptr->data, g_src_attributed, history_ncells * sizeof(uint16_t)));
	UT_LOG_CALL(g_ret = video_text_history_read(&g_video_text_test, rewind_ncells, g_result, history_ncells));
	UT_EXPECT_EQMEM(g_history_expect.data, g_result, history_ncells * sizeof(uint16_t));
	UT_EXPECT_EQ(0, g_ret);
}


/*
 * TODO:
 * */
UT_CREATE_SUITE(video_text_history, set, "Test history set functionality")
UT_CREATE_CASE(video_text_history, set, no_wrap, "Setting data without history wrap")
{
	const uint32_t	nborder_left = 1;
	const uint32_t	nborder_right = 1;
	const uint32_t	ncells_to_set = VIDEO_TEXT_HISTORY_1_PERCENT;
	const uint32_t	ncells_to_cmp = nborder_left + ncells_to_set + nborder_right;

	history_test_init(g_history_test_ptr, nborder_left, 0);
	history_expect_side_effects_append(&g_history_expect, g_history_test_ptr, ncells_to_set);

	memcpy(g_history_expect.data, g_history_test_ptr->data, nborder_left * sizeof(uint16_t));
	memsetw(&g_history_expect.data[nborder_left], SRC_UNIFORM_WORD, ncells_to_set);
	memcpy(&g_history_expect.data[nborder_left + ncells_to_set], &g_history_test_ptr->data[nborder_left + ncells_to_set], nborder_right * sizeof(uint16_t));
	UT_LOG_CALL(g_ret = video_text_history_set(&g_video_text_test, 0, SRC_UNIFORM_WORD, ncells_to_set));

	UT_EXPECT_EQMEM(g_history_expect.data, g_history_test_ptr->data, ncells_to_cmp * sizeof(uint16_t));
	UT_EXPECT_EQ(ncells_to_set, g_ret);
	history_expect_side_effects_compare(&g_history_expect, g_history_test_ptr);
}
UT_CREATE_CASE(video_text_history, set, with_wrap, "Setting data with history wrap")
{
	const uint32_t	nborder_left = 1;
	const uint32_t	nborder_right = 1;
	const uint32_t	ncells_to_set = VIDEO_TEXT_HISTORY_1_PERCENT;
	const uint32_t	ncells_to_cmp = nborder_left + ncells_to_set + nborder_right;
	const uint32_t	chunk1_ncells = ncells_to_set / 2;
	const uint32_t	chunk2_ncells = ncells_to_set - chunk1_ncells;
	const uint32_t	initial_offset_cell = VIDEO_TEXT_HISTORY_MAX - chunk1_ncells;

	history_test_init(g_history_test_ptr, initial_offset_cell, 0);
	history_expect_side_effects_append(&g_history_expect, g_history_test_ptr, ncells_to_set);

	memcpy(g_history_expect.data, &g_history_test_ptr->data[initial_offset_cell - nborder_left], nborder_left * sizeof(uint16_t));
	memsetw(&g_history_expect.data[nborder_left], SRC_UNIFORM_WORD, ncells_to_set);
	memcpy(&g_history_expect.data[nborder_left + ncells_to_set], &g_history_test_ptr->data[chunk2_ncells], nborder_right * sizeof(uint16_t));
	UT_LOG_CALL(g_ret = video_text_history_set(&g_video_text_test, 0, SRC_UNIFORM_WORD, ncells_to_set));
	memcpy(g_result, &g_history_test_ptr->data[initial_offset_cell - nborder_left], nborder_left * sizeof(uint16_t));
	memcpy(&g_result[nborder_left], &g_history_test_ptr->data[initial_offset_cell], chunk1_ncells * sizeof(uint16_t));
	memcpy(&g_result[nborder_left + chunk1_ncells], g_history_test_ptr->data, chunk2_ncells * sizeof(uint16_t));
	memcpy(&g_result[nborder_left + ncells_to_set], &g_history_test_ptr->data[chunk2_ncells], nborder_right * sizeof(uint16_t));

	UT_EXPECT_EQMEM(g_history_expect.data, g_result, ncells_to_cmp * sizeof(uint16_t));
	UT_EXPECT_EQ(ncells_to_set, g_ret);
	history_expect_side_effects_compare(&g_history_expect, g_history_test_ptr);
}
UT_CREATE_CASE(video_text_history, set, update_data_only, "Update old data")
{
	const uint32_t	history_ncells = VIDEO_TEXT_HISTORY_1_PERCENT;
	const uint32_t	offset_cell = history_ncells % VIDEO_TEXT_HISTORY_MAX;
	const uint32_t	rewind_ncells = history_ncells;

	history_test_init(g_history_test_ptr, offset_cell, history_ncells);
	history_expect_side_effects_append(&g_history_expect, g_history_test_ptr, 0);

	UT_LOG_CALL(g_ret = video_text_history_set(&g_video_text_test, rewind_ncells, SRC_UNIFORM_WORD, history_ncells));

	UT_EXPECT_EQMEM(g_src_uniform_words, g_history_test_ptr->data, history_ncells * sizeof(uint16_t));
	UT_EXPECT_EQ(history_ncells, g_ret);
	history_expect_side_effects_compare(&g_history_expect, g_history_test_ptr);
}
UT_CREATE_CASE(video_text_history, set, append_data_only, "Append new data")
{
	const uint32_t	ncells = VIDEO_TEXT_HISTORY_1_PERCENT;

	history_test_init(g_history_test_ptr, 0, 0);
	history_expect_side_effects_append(&g_history_expect, g_history_test_ptr, ncells);

	UT_LOG_CALL(g_ret = video_text_history_set(&g_video_text_test, 0, SRC_UNIFORM_WORD, ncells));

	UT_EXPECT_EQMEM(g_src_uniform_words, g_history_test_ptr->data, ncells * sizeof(uint16_t));
	UT_EXPECT_EQ(ncells, g_ret);
	history_expect_side_effects_compare(&g_history_expect, g_history_test_ptr);
}
UT_CREATE_CASE(video_text_history, set, update_and_append_data, "Update old data and append new data")
{
	const uint32_t	history_ncells = VIDEO_TEXT_HISTORY_1_PERCENT;
	const uint32_t	append_ncells = 1;
	const uint32_t	total_ncells = history_ncells + append_ncells;
	const uint32_t	offset_cell = history_ncells % VIDEO_TEXT_HISTORY_MAX;
	const uint32_t	rewind_ncells = history_ncells;

	history_test_init(g_history_test_ptr, offset_cell, history_ncells);
	history_expect_side_effects_append(&g_history_expect, g_history_test_ptr, append_ncells);

	UT_LOG_CALL(g_ret = video_text_history_set(&g_video_text_test, rewind_ncells, SRC_UNIFORM_WORD, total_ncells));

	UT_EXPECT_EQMEM(g_src_uniform_words, g_history_test_ptr->data, total_ncells);
	UT_EXPECT_EQ(total_ncells, g_ret);
	history_expect_side_effects_compare(&g_history_expect, g_history_test_ptr);
}
UT_CREATE_CASE(video_text_history, set, over_history_max, "Setting more than VIDEO_TEXT_HISTORY_MAX cells")
{
	const uint32_t	overwrite_ncells = 1;
	const uint32_t	oversized_ncells = VIDEO_TEXT_HISTORY_MAX + overwrite_ncells;

	vga_text_print("\n// With rewind < overwrite_ncells\n", VGA_TEXT_LIGHT_MAGENTA);
	history_test_init(g_history_test_ptr, 0, 0);
	history_expect_side_effects_append(&g_history_expect, g_history_test_ptr, oversized_ncells);

	UT_LOG_CALL(g_ret = video_text_history_set(&g_video_text_test, 0, SRC_UNIFORM_WORD, oversized_ncells));

	UT_EXPECT_EQMEM(g_src_uniform_words, g_history_test_ptr->data, VIDEO_TEXT_HISTORY_MAX * sizeof(uint16_t));
	UT_EXPECT_EQ(VIDEO_TEXT_HISTORY_MAX, g_ret);
	history_expect_side_effects_compare(&g_history_expect, g_history_test_ptr);

	const uint32_t	rewind_ncells = VIDEO_TEXT_HISTORY_MAX;

	vga_text_print("\n// With rewind > overwrite_ncells\n", VGA_TEXT_LIGHT_MAGENTA);
	history_test_init(g_history_test_ptr, 0, VIDEO_TEXT_HISTORY_MAX);
	history_expect_side_effects_append(&g_history_expect, g_history_test_ptr, oversized_ncells - rewind_ncells);

	UT_LOG_CALL(g_ret = video_text_history_set(&g_video_text_test, rewind_ncells, SRC_UNIFORM_WORD, oversized_ncells));

	UT_EXPECT_EQMEM(g_src_uniform_words, g_history_test_ptr->data, VIDEO_TEXT_HISTORY_MAX * sizeof(uint16_t));
	UT_EXPECT_EQ(VIDEO_TEXT_HISTORY_MAX, g_ret);
	history_expect_side_effects_compare(&g_history_expect, g_history_test_ptr);
}
UT_CREATE_CASE(video_text_history, set, in_bounds_rewind, "Setting data with rewind <= current history ncells")
{
	const uint32_t	history_ncells = VIDEO_TEXT_HISTORY_1_PERCENT;
	const uint32_t	rewind_ncells = history_ncells;

	history_test_init(g_history_test_ptr, history_ncells, history_ncells);
	history_expect_side_effects_append(&g_history_expect, g_history_test_ptr, history_ncells - rewind_ncells);

	UT_LOG_CALL(g_ret = video_text_history_set(&g_video_text_test, rewind_ncells, SRC_UNIFORM_WORD, history_ncells));

	UT_EXPECT_EQMEM(g_src_uniform_words, g_history_test_ptr->data, history_ncells * sizeof(uint16_t));
	UT_EXPECT_EQ(history_ncells, g_ret);
	history_expect_side_effects_compare(&g_history_expect, g_history_test_ptr);
}
UT_CREATE_CASE(video_text_history, set, out_of_bounds_rewind, "Setting data with rewind > current history ncells")
{
	const uint32_t	history_ncells = VIDEO_TEXT_HISTORY_1_PERCENT;
	const uint32_t	rewind_ncells = history_ncells + 1;

	history_test_init(g_history_test_ptr, history_ncells, history_ncells);
	history_expect_side_effects_append(&g_history_expect, g_history_test_ptr, 0);

	UT_LOG_CALL(memset(g_history_expect.data, 0, history_ncells * sizeof(uint16_t)));
	UT_LOG_CALL(g_ret = video_text_history_set(&g_video_text_test, rewind_ncells, SRC_UNIFORM_WORD, history_ncells));

	UT_EXPECT_EQMEM(g_history_expect.data, g_history_test_ptr->data, history_ncells * sizeof(uint16_t));
	UT_EXPECT_EQ(0, g_ret);
	history_expect_side_effects_compare(&g_history_expect, g_history_test_ptr);
}
