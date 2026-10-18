/*
 * File: test_video_text_history.c
 * Author: Joel Souza
 * Date: 2026-07-30
 * Description: Test video text history management functions
 */

# include "drivers/video/text/config.h"
# include "drivers/video/text/video_text.h"
# include "test/unit_test/unit_test.h"
# include "test/drivers/video/text/test_video_text_history.h"

UT_CREATE_CATEGORY(video_text_history, "Test Video Text History functionalities")

static char					g_src_raw[SRC_SIZE];
static uint16_t				g_src_attributed[SRC_SIZE];
static uint16_t				g_src_uniform_words[SRC_SIZE];
static uint16_t				g_result[VIDEO_TEXT_HISTORY_MAX];

const uint32_t				VIDEO_TEXT_HISTORY_1_PERCENT = VIDEO_TEXT_HISTORY_MAX * 0.01;

__attribute__((constructor))
static void	test_video_text_history_src_init(void)
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
static t_video_text_history			*g_history_test = &g_video_text_test.history;
static const t_video_text_config	*g_config = &g_video_text_config_default;

static
void	init_history_test(uint32_t offset, uint32_t size)
{
	g_history_test->offset = offset;
	g_history_test->line_offset = offset % g_config->width;
	g_history_test->size = size;
	g_history_test->nlines = align_up(size, g_config->width) / g_config->width;
	memset(g_history_test->data, 0, VIDEO_TEXT_HISTORY_MAX * sizeof(uint16_t));
	memset(g_result, 0, VIDEO_TEXT_HISTORY_MAX * sizeof(uint16_t));
}

static uint32_t	g_expected_offset;
static uint32_t	g_expected_line_offset;
static uint32_t	g_expected_size;
static uint32_t	g_expected_nlines;
static uint16_t	g_expected_data[VIDEO_TEXT_HISTORY_MAX];
static uint32_t	g_ret;

static
void	simulate_side_effects(uint32_t nentries)
{
	g_expected_offset = (g_history_test->offset + nentries) % VIDEO_TEXT_HISTORY_MAX;
	g_expected_line_offset = g_expected_offset % g_config->width;
	g_expected_size = MIN(g_history_test->size + nentries, VIDEO_TEXT_HISTORY_MAX);
	g_expected_nlines = align_up(g_expected_size, g_config->width) / g_config->width;
}

static
void	test_side_effects(void)
{
	UT_EXPECT_EQ(g_expected_offset, g_history_test->offset);
	UT_EXPECT_EQ(g_expected_line_offset, g_history_test->line_offset);
	UT_EXPECT_EQ(g_expected_size, g_history_test->size);
	UT_EXPECT_EQ(g_expected_nlines, g_history_test->nlines);
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
	const uint32_t	nentries = VIDEO_TEXT_HISTORY_1_PERCENT;

	init_history_test(0, 0);
	simulate_side_effects(nentries);
	UT_LOG_CALL(g_ret = video_text_history_write(&g_video_text_test, g_src_raw, nentries, SRC_ATTR));
	UT_EXPECT_EQMEM(g_src_attributed, g_history_test->data, nentries * sizeof(uint16_t));
	UT_EXPECT_EQ(nentries, g_ret);
	test_side_effects();
}
UT_CREATE_CASE(video_text_history, write, with_wrap, "Writing data with history wrap")
{
	const uint32_t	total_nentries = VIDEO_TEXT_HISTORY_1_PERCENT;
	const uint32_t	chunk1_nentries = total_nentries / 2;
	const uint32_t	chunk2_nentries = total_nentries - chunk1_nentries;
	const uint32_t	initial_offset = VIDEO_TEXT_HISTORY_MAX - chunk1_nentries;

	init_history_test(initial_offset, 0);
	simulate_side_effects(total_nentries);
	UT_LOG_CALL(g_ret = video_text_history_write(&g_video_text_test, g_src_raw, total_nentries, SRC_ATTR));
	UT_LOG_CALL(memcpy(g_result, &g_history_test->data[initial_offset], chunk1_nentries * sizeof(uint16_t)));
	UT_LOG_CALL(memcpy(&g_result[chunk1_nentries], g_history_test->data, chunk2_nentries * sizeof(uint16_t)));
	UT_EXPECT_EQMEM(g_src_attributed, g_result, total_nentries * sizeof(uint16_t));
	UT_EXPECT_EQ(total_nentries, g_ret);
	test_side_effects();
}
UT_CREATE_CASE(video_text_history, write, over_history_max, "Writing more than VIDEO_TEXT_HISTORY_MAX entries")
{
	const uint32_t	overwrite_nentries = 1;
	const uint32_t	oversized_nentries = VIDEO_TEXT_HISTORY_MAX + overwrite_nentries;
	const uint32_t	remaining_nentries = VIDEO_TEXT_HISTORY_MAX - overwrite_nentries;

	init_history_test(0, 0);
	simulate_side_effects(oversized_nentries);
	UT_LOG_CALL(g_ret = video_text_history_write(&g_video_text_test, g_src_raw, oversized_nentries, SRC_ATTR));
	UT_LOG_CALL(memcpy(g_result, &g_history_test->data[overwrite_nentries], remaining_nentries * sizeof(uint16_t)));
	UT_LOG_CALL(memcpy(&g_result[remaining_nentries], g_history_test->data, overwrite_nentries * sizeof(uint16_t)));
	UT_EXPECT_EQMEM(&g_src_attributed[overwrite_nentries], g_result, VIDEO_TEXT_HISTORY_MAX * sizeof(uint16_t));
	UT_EXPECT_EQ(VIDEO_TEXT_HISTORY_MAX, g_ret);
	test_side_effects();
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
	const uint32_t	nentries = VIDEO_TEXT_HISTORY_1_PERCENT;
	const uint32_t	offset = nentries % VIDEO_TEXT_HISTORY_MAX;

	init_history_test(offset, nentries);
	UT_LOG_CALL(memcpy(g_history_test->data, g_src_attributed, nentries * sizeof(uint16_t)));
	UT_LOG_CALL(g_ret = video_text_history_read(&g_video_text_test, 0, g_result, nentries));
	UT_EXPECT_EQMEM(g_src_attributed, g_result, nentries * sizeof(uint16_t));
	UT_EXPECT_EQ(nentries, g_ret);
}
UT_CREATE_CASE(video_text_history, read, with_wrap, "Reading data with history wrap")
{
	const uint32_t	total_nentries = VIDEO_TEXT_HISTORY_1_PERCENT;
	const uint32_t	chunk1_nentries = total_nentries / 2;
	const uint32_t	chunk2_nentries = total_nentries - chunk1_nentries;
	const uint32_t	initial_offset = VIDEO_TEXT_HISTORY_MAX - chunk1_nentries;

	init_history_test(chunk2_nentries, total_nentries);
	UT_LOG_CALL(memcpy(&g_history_test->data[initial_offset], g_src_attributed, chunk1_nentries * sizeof(uint16_t)));
	UT_LOG_CALL(memcpy(g_history_test->data, &g_src_attributed[chunk1_nentries], chunk2_nentries * sizeof(uint16_t)));
	UT_LOG_CALL(g_ret = video_text_history_read(&g_video_text_test, 0, g_result, total_nentries));
	UT_EXPECT_EQMEM(g_src_attributed, g_result, total_nentries * sizeof(uint16_t));
	UT_EXPECT_EQ(total_nentries, g_ret);
}
UT_CREATE_CASE(video_text_history, read, over_history_size, "Reading more than the current history size")
{
	const uint32_t	history_size = VIDEO_TEXT_HISTORY_1_PERCENT;
	const uint32_t	offset = history_size % VIDEO_TEXT_HISTORY_MAX;
	const uint32_t	extra_nentries = 1;
	const uint32_t	oversized_nentries = history_size + extra_nentries;

	init_history_test(offset, history_size);
	memcpy(g_expected_data, g_src_attributed, history_size * sizeof(uint16_t));
	memcpy(&g_expected_data[history_size], &g_result[history_size], extra_nentries * sizeof(uint16_t));
	UT_LOG_CALL(memcpy(g_history_test->data, g_src_attributed, history_size * sizeof(uint16_t)));
	UT_LOG_CALL(g_ret = video_text_history_read(&g_video_text_test, 0, g_result, oversized_nentries));
	UT_EXPECT_EQMEM(g_expected_data, g_result, oversized_nentries * sizeof(uint16_t));
	UT_EXPECT_EQ(history_size, g_ret);
}
UT_CREATE_CASE(video_text_history, read, in_bounds_rewind, "Reading data with rewind < history->size")
{
	const uint32_t	history_size = VIDEO_TEXT_HISTORY_1_PERCENT;
	const uint32_t	offset = history_size % VIDEO_TEXT_HISTORY_MAX;
	const uint32_t	remaining_nentries = 1;
	const uint32_t	rewind = history_size - remaining_nentries;

	init_history_test(offset, history_size);
	memcpy(g_expected_data, g_src_attributed, remaining_nentries * sizeof(uint16_t));
	memcpy(&g_expected_data[remaining_nentries], &g_result[remaining_nentries], rewind * sizeof(uint16_t));
	UT_LOG_CALL(memcpy(g_history_test->data, g_src_attributed, history_size * sizeof(uint16_t)));
	UT_LOG_CALL(g_ret = video_text_history_read(&g_video_text_test, rewind, g_result, history_size));
	UT_EXPECT_EQMEM(g_expected_data, g_result, history_size * sizeof(uint16_t));
	UT_EXPECT_EQ(remaining_nentries, g_ret);
}
UT_CREATE_CASE(video_text_history, read, out_of_bounds_rewind, "Reading data with rewind >= history->size")
{
	const uint32_t	history_size = VIDEO_TEXT_HISTORY_1_PERCENT;
	const uint32_t	offset = history_size % VIDEO_TEXT_HISTORY_MAX;
	const uint32_t	rewind = history_size;
	uint16_t		result_backup[history_size];

	init_history_test(offset, history_size);
	memcpy(result_backup, g_result, history_size * sizeof(uint16_t));
	UT_LOG_CALL(memcpy(g_history_test->data, g_src_attributed, history_size * sizeof(uint16_t)));
	UT_LOG_CALL(g_ret = video_text_history_read(&g_video_text_test, rewind, g_result, history_size));
	UT_EXPECT_EQMEM(result_backup, g_result, history_size * sizeof(uint16_t));
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
	const uint32_t	nentries_to_set = VIDEO_TEXT_HISTORY_1_PERCENT;
	const uint32_t	nentries_to_cmp = nborder_left + nentries_to_set + nborder_right;

	init_history_test(nborder_left, 0);
	simulate_side_effects(nentries_to_set);
	memcpy(g_expected_data, g_history_test->data, nborder_left * sizeof(uint16_t));
	memsetw(&g_expected_data[nborder_left], SRC_UNIFORM_WORD, nentries_to_set);
	memcpy(&g_expected_data[nborder_left + nentries_to_set], &g_history_test->data[nborder_left + nentries_to_set], nborder_right * sizeof(uint16_t));
	UT_LOG_CALL(g_ret = video_text_history_set(&g_video_text_test, 0, SRC_UNIFORM_WORD, nentries_to_set));
	UT_EXPECT_EQMEM(g_expected_data, g_history_test->data, nentries_to_cmp * sizeof(uint16_t));
	UT_EXPECT_EQ(nentries_to_set, g_ret);
	test_side_effects();
}
UT_CREATE_CASE(video_text_history, set, with_wrap, "Setting data with history wrap")
{
	const uint32_t	nborder_left = 1;
	const uint32_t	nborder_right = 1;
	const uint32_t	nentries_to_set = VIDEO_TEXT_HISTORY_1_PERCENT;
	const uint32_t	nentries_to_cmp = nborder_left + nentries_to_set + nborder_right;
	const uint32_t	chunk1_nentries = nentries_to_set / 2;
	const uint32_t	chunk2_nentries = nentries_to_set - chunk1_nentries;
	const uint32_t	initial_offset = VIDEO_TEXT_HISTORY_MAX - chunk1_nentries;

	init_history_test(initial_offset, 0);
	simulate_side_effects(nentries_to_set);
	memcpy(g_expected_data, &g_history_test->data[initial_offset - nborder_left], nborder_left * sizeof(uint16_t));
	memsetw(&g_expected_data[nborder_left], SRC_UNIFORM_WORD, nentries_to_set);
	memcpy(&g_expected_data[nborder_left + nentries_to_set], &g_history_test->data[chunk2_nentries], nborder_right * sizeof(uint16_t));
	UT_LOG_CALL(g_ret = video_text_history_set(&g_video_text_test, 0, SRC_UNIFORM_WORD, nentries_to_set));
	memcpy(g_result, &g_history_test->data[initial_offset - nborder_left], nborder_left * sizeof(uint16_t));
	memcpy(&g_result[nborder_left], &g_history_test->data[initial_offset], chunk1_nentries * sizeof(uint16_t));
	memcpy(&g_result[nborder_left + chunk1_nentries], g_history_test->data, chunk2_nentries * sizeof(uint16_t));
	memcpy(&g_result[nborder_left + nentries_to_set], &g_history_test->data[chunk2_nentries], nborder_right * sizeof(uint16_t));
	UT_EXPECT_EQMEM(g_expected_data, g_result, nentries_to_cmp * sizeof(uint16_t));
	UT_EXPECT_EQ(nentries_to_set, g_ret);
	test_side_effects();
}
UT_CREATE_CASE(video_text_history, set, update_data_only, "Update old data")
{
	const uint32_t	history_size = VIDEO_TEXT_HISTORY_1_PERCENT;
	const uint32_t	offset = history_size % VIDEO_TEXT_HISTORY_MAX;
	const uint32_t	rewind = history_size;

	init_history_test(offset, history_size);
	simulate_side_effects(0);
	UT_LOG_CALL(g_ret = video_text_history_set(&g_video_text_test, rewind, SRC_UNIFORM_WORD, history_size));
	UT_EXPECT_EQMEM(g_src_uniform_words, g_history_test->data, history_size * sizeof(uint16_t));
	UT_EXPECT_EQ(history_size, g_ret);
	test_side_effects();
}
UT_CREATE_CASE(video_text_history, set, append_data_only, "Append new data")
{
	const uint32_t	nentries = VIDEO_TEXT_HISTORY_1_PERCENT;

	init_history_test(0, 0);
	simulate_side_effects(nentries);
	UT_LOG_CALL(g_ret = video_text_history_set(&g_video_text_test, 0, SRC_UNIFORM_WORD, nentries));
	UT_EXPECT_EQMEM(g_src_uniform_words, g_history_test->data, nentries * sizeof(uint16_t));
	UT_EXPECT_EQ(nentries, g_ret);
	test_side_effects();
}
UT_CREATE_CASE(video_text_history, set, update_and_append_data, "Update old data and append new data")
{
	const uint32_t	history_size = VIDEO_TEXT_HISTORY_1_PERCENT;
	const uint32_t	nappend_data = 1;
	const uint32_t	nentries = history_size + nappend_data;
	const uint32_t	offset = history_size % VIDEO_TEXT_HISTORY_MAX;
	const uint32_t	rewind = history_size;

	init_history_test(offset, history_size);
	simulate_side_effects(nappend_data);
	UT_LOG_CALL(g_ret = video_text_history_set(&g_video_text_test, rewind, SRC_UNIFORM_WORD, nentries));
	UT_EXPECT_EQMEM(g_src_uniform_words, g_history_test->data, nentries);
	UT_EXPECT_EQ(nentries, g_ret);
	test_side_effects();
}
UT_CREATE_CASE(video_text_history, set, over_history_max, "Setting more than VIDEO_TEXT_HISTORY_MAX entries")
{
	const uint32_t	overwrite_nentries = 1;
	const uint32_t	oversized_nentries = VIDEO_TEXT_HISTORY_MAX + overwrite_nentries;

	vga_text_print("\n// With rewind < overwrite_nentries\n", VGA_TEXT_LIGHT_MAGENTA);
	init_history_test(0, 0);
	simulate_side_effects(oversized_nentries);
	UT_LOG_CALL(g_ret = video_text_history_set(&g_video_text_test, 0, SRC_UNIFORM_WORD, oversized_nentries));
	UT_EXPECT_EQMEM(g_src_uniform_words, g_history_test->data, VIDEO_TEXT_HISTORY_MAX * sizeof(uint16_t));
	UT_EXPECT_EQ(VIDEO_TEXT_HISTORY_MAX, g_ret);
	test_side_effects();

	const uint32_t	rewind = VIDEO_TEXT_HISTORY_MAX;

	vga_text_print("\n// With rewind > overwrite_nentries\n", VGA_TEXT_LIGHT_MAGENTA);
	init_history_test(0, VIDEO_TEXT_HISTORY_MAX);
	simulate_side_effects(oversized_nentries - rewind);
	UT_LOG_CALL(g_ret = video_text_history_set(&g_video_text_test, rewind, SRC_UNIFORM_WORD, oversized_nentries));
	UT_EXPECT_EQMEM(g_src_uniform_words, g_history_test->data, VIDEO_TEXT_HISTORY_MAX * sizeof(uint16_t));
	UT_EXPECT_EQ(VIDEO_TEXT_HISTORY_MAX, g_ret);
	test_side_effects();
}
UT_CREATE_CASE(video_text_history, set, in_bounds_rewind, "Setting data with rewind <= history->size")
{
	const uint32_t	history_size = VIDEO_TEXT_HISTORY_1_PERCENT;
	const uint32_t	rewind = history_size;

	init_history_test(history_size, history_size);
	simulate_side_effects(history_size - rewind);
	UT_LOG_CALL(g_ret = video_text_history_set(&g_video_text_test, rewind, SRC_UNIFORM_WORD, history_size));
	UT_EXPECT_EQMEM(g_src_uniform_words, g_history_test->data, history_size * sizeof(uint16_t));
	UT_EXPECT_EQ(history_size, g_ret);
	test_side_effects();
}
UT_CREATE_CASE(video_text_history, set, out_of_bounds_rewind, "Setting data with rewind > history->size")
{
	const uint32_t	history_size = VIDEO_TEXT_HISTORY_1_PERCENT;
	const uint32_t	rewind = history_size + 1;

	init_history_test(history_size, history_size);
	simulate_side_effects(0);
	UT_LOG_CALL(memset(g_expected_data, 0, history_size * sizeof(uint16_t)));
	UT_LOG_CALL(g_ret = video_text_history_set(&g_video_text_test, rewind, SRC_UNIFORM_WORD, history_size));
	UT_EXPECT_EQMEM(g_expected_data, g_history_test->data, history_size * sizeof(uint16_t));
	UT_EXPECT_EQ(0, g_ret);
	test_side_effects();
}
