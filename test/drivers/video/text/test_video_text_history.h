/*
 * File: test_video_text_history.h
 * Author: Joel Souza
 * Date: 2026-05-08
 * Description: Test video text history management functions
 */

#ifndef TEST_VIDEO_TEXT_HISTORY_H
# define TEST_VIDEO_TEXT_HISTORY_H

# include "drivers/video/text/config.h"
# include "drivers/video/text/video_text.h"
# include "test/unit_test/unit_test.h"

UT_CREATE_CATEGORY(video_text_history, "Test Video Text History functionalities")

# define SRC_PATTERN	"This is a Test!"
# define SRC_ATTR		'.'
enum {SRC_SIZE = VIDEO_TEXT_HISTORY_MAX * 2};

static char				src_raw[SRC_SIZE];
static uint16_t			src_attributed[SRC_SIZE];
static uint16_t			result[VIDEO_TEXT_HISTORY_MAX];

const uint32_t			VIDEO_TEXT_HISTORY_1_PERCENT = VIDEO_TEXT_HISTORY_MAX * 0.01;

__attribute__((constructor))
static void	src_init(void)
{
	const uint32_t	src_pattern_bytes = strlen(SRC_PATTERN);
	uint32_t		bytes_to_fill;
	void			*ptr;

	ptr = src_raw;
	bytes_to_fill = SRC_SIZE;
	while (bytes_to_fill >= src_pattern_bytes)
	{
		ptr = mempcpy(ptr, SRC_PATTERN, src_pattern_bytes);
		bytes_to_fill -= src_pattern_bytes;
	}
	if (bytes_to_fill > 0)
		memcpy(ptr, SRC_PATTERN, bytes_to_fill);
	memicpy8(src_attributed, src_raw, SRC_SIZE, SRC_ATTR);
}

static t_video_text_history	history_test;

static
void	init_history_test(uint32_t offset, uint32_t size)
{
	history_test.offset = offset;
	history_test.line_offset = offset % g_video_text_config.width;
	history_test.size = size;
	history_test.lines = align_up(size, g_video_text_config.width) / g_video_text_config.width;
	memset(history_test.data, 0, VIDEO_TEXT_HISTORY_MAX * sizeof(uint16_t));
	memset(result, 0, VIDEO_TEXT_HISTORY_MAX * sizeof(uint16_t));
}

static uint32_t	expected_offset;
static uint32_t	expected_line_offset;
static uint32_t	expected_size;
static uint32_t	expected_lines;
static uint32_t	ret;

static
void	simulate_side_effects(uint32_t nbytes)
{
	expected_offset = (history_test.offset + nbytes) % VIDEO_TEXT_HISTORY_MAX;
	expected_line_offset = expected_offset % g_video_text_config.width;
	expected_size = MIN(history_test.size + nbytes, VIDEO_TEXT_HISTORY_MAX);
	expected_lines = align_up(expected_size, g_video_text_config.width) / g_video_text_config.width;
}

static
void	test_side_effects(void)
{
	UT_EXPECT_EQ(expected_offset, history_test.offset);
	UT_EXPECT_EQ(expected_line_offset, history_test.line_offset);
	UT_EXPECT_EQ(expected_size, history_test.size);
	UT_EXPECT_EQ(expected_lines, history_test.lines);
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
	const uint32_t	nbytes = VIDEO_TEXT_HISTORY_1_PERCENT;

	init_history_test(0, 0);
	simulate_side_effects(nbytes);
	UT_LOG_CALL(ret = video_text_history_write(&history_test, src_raw, nbytes, SRC_ATTR));
	UT_EXPECT_EQMEM(src_attributed, history_test.data, nbytes * sizeof(uint16_t));
	UT_EXPECT_EQ(nbytes, ret);
	test_side_effects();
}
UT_CREATE_CASE(video_text_history, write, with_wrap, "Writing data with history wrap")
{
	const uint32_t	total_nbytes = VIDEO_TEXT_HISTORY_1_PERCENT;
	const uint32_t	chunk1_nbytes = total_nbytes / 2;
	const uint32_t	chunk2_nbytes = total_nbytes - chunk1_nbytes;
	const uint32_t	initial_offset = VIDEO_TEXT_HISTORY_MAX - chunk1_nbytes;

	init_history_test(initial_offset, 0);
	simulate_side_effects(total_nbytes);
	UT_LOG_CALL(ret = video_text_history_write(&history_test, src_raw, total_nbytes, SRC_ATTR));
	UT_LOG_CALL(memcpy(result, &history_test.data[initial_offset], chunk1_nbytes * sizeof(uint16_t)));
	UT_LOG_CALL(memcpy(&result[chunk1_nbytes], history_test.data, chunk2_nbytes * sizeof(uint16_t)));
	UT_EXPECT_EQMEM(src_attributed, result, total_nbytes * sizeof(uint16_t));
	UT_EXPECT_EQ(total_nbytes, ret);
	test_side_effects();
}
UT_CREATE_CASE(video_text_history, write, overwrite_oldest, "Test history overwrite by passing nbytes > VIDEO_TEXT_HISTORY_MAX")
{
	const uint32_t	overwrite_nbytes = 17;
	const uint32_t	oversized_nbytes = VIDEO_TEXT_HISTORY_MAX + overwrite_nbytes;
	const uint32_t	remaining_nbytes = VIDEO_TEXT_HISTORY_MAX - overwrite_nbytes;

	init_history_test(0, 0);
	simulate_side_effects(oversized_nbytes);
	UT_LOG_CALL(ret = video_text_history_write(&history_test, src_raw, oversized_nbytes, SRC_ATTR));
	UT_LOG_CALL(memcpy(result, &history_test.data[overwrite_nbytes], remaining_nbytes * sizeof(uint16_t)));
	UT_LOG_CALL(memcpy(&result[remaining_nbytes], history_test.data, overwrite_nbytes * sizeof(uint16_t)));
	UT_EXPECT_EQMEM(&src_attributed[overwrite_nbytes], result, VIDEO_TEXT_HISTORY_MAX * sizeof(uint16_t));
	UT_EXPECT_EQ(VIDEO_TEXT_HISTORY_MAX, ret);
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
	const uint32_t	nbytes = VIDEO_TEXT_HISTORY_1_PERCENT;
	const uint32_t	offset = nbytes % VIDEO_TEXT_HISTORY_MAX;

	init_history_test(offset, nbytes);
	UT_LOG_CALL(memcpy(history_test.data, src_attributed, nbytes * sizeof(uint16_t)));
	UT_LOG_CALL(ret = video_text_history_read(&history_test, 0, result, nbytes));
	UT_EXPECT_EQMEM(src_attributed, result, nbytes * sizeof(uint16_t));
	UT_EXPECT_EQ(nbytes, ret);
}
UT_CREATE_CASE(video_text_history, read, with_wrap, "Reading data with history wrap")
{
	const uint32_t	total_nbytes = VIDEO_TEXT_HISTORY_1_PERCENT;
	const uint32_t	chunk1_nbytes = total_nbytes / 2;
	const uint32_t	chunk2_nbytes = total_nbytes - chunk1_nbytes;
	const uint32_t	initial_offset = VIDEO_TEXT_HISTORY_MAX - chunk1_nbytes;

	init_history_test(chunk2_nbytes, total_nbytes);
	UT_LOG_CALL(memcpy(&history_test.data[initial_offset], src_attributed, chunk1_nbytes * sizeof(uint16_t)));
	UT_LOG_CALL(memcpy(history_test.data, &src_attributed[chunk1_nbytes], chunk2_nbytes * sizeof(uint16_t)));
	UT_LOG_CALL(ret = video_text_history_read(&history_test, 0, result, total_nbytes));
	UT_EXPECT_EQMEM(src_attributed, result, total_nbytes * sizeof(uint16_t));
	UT_EXPECT_EQ(total_nbytes, ret);
}
UT_CREATE_CASE(video_text_history, read, over_history_size, "Reading more than the current history size")
{
	const uint32_t	history_size = VIDEO_TEXT_HISTORY_1_PERCENT;
	const uint32_t	offset = history_size % VIDEO_TEXT_HISTORY_MAX;
	const uint32_t	extra_nbytes = 1;
	const uint32_t	oversized_nbytes = history_size + extra_nbytes;
	uint16_t		expected_result[oversized_nbytes];

	init_history_test(offset, history_size);
	memcpy(expected_result, src_attributed, history_size * sizeof(uint16_t));
	memcpy(&expected_result[history_size], &result[history_size], extra_nbytes * sizeof(uint16_t));
	UT_LOG_CALL(memcpy(history_test.data, src_attributed, history_size * sizeof(uint16_t)));
	UT_LOG_CALL(ret = video_text_history_read(&history_test, 0, result, oversized_nbytes));
	UT_EXPECT_EQMEM(expected_result, result, oversized_nbytes * sizeof(uint16_t));
	UT_EXPECT_EQ(history_size, ret);
}
UT_CREATE_CASE(video_text_history, read, with_rewind, "Reading data with non-zero rewind")
{
	const uint32_t	history_size = VIDEO_TEXT_HISTORY_1_PERCENT;
	const uint32_t	offset = history_size % VIDEO_TEXT_HISTORY_MAX;
	const uint32_t	remaining_nbytes = history_size - 1;
	const uint32_t	rewind = history_size - remaining_nbytes;
	uint16_t		expected_result[history_size];

	init_history_test(offset, history_size);
	memcpy(expected_result, src_attributed, remaining_nbytes * sizeof(uint16_t));
	memcpy(&expected_result[remaining_nbytes], &result[remaining_nbytes], rewind * sizeof(uint16_t));
	UT_LOG_CALL(memcpy(history_test.data, src_attributed, history_size * sizeof(uint16_t)));
	UT_LOG_CALL(ret = video_text_history_read(&history_test, rewind, result, history_size));
	UT_EXPECT_EQMEM(expected_result, result, history_size * sizeof(uint16_t));
	UT_EXPECT_EQ(remaining_nbytes, ret);
}
UT_CREATE_CASE(video_text_history, read, rewind_over_history_size, "Reading with rewind >= history->size")
{
	const uint32_t	history_size = VIDEO_TEXT_HISTORY_1_PERCENT;
	const uint32_t	offset = history_size % VIDEO_TEXT_HISTORY_MAX;
	const uint32_t	rewind = history_size;
	uint16_t		result_backup[history_size];

	init_history_test(offset, history_size);
	memcpy(result_backup, result, history_size * sizeof(uint16_t));
	UT_LOG_CALL(memcpy(history_test.data, src_attributed, history_size * sizeof(uint16_t)));
	UT_LOG_CALL(ret = video_text_history_read(&history_test, rewind, result, history_size));
	UT_EXPECT_EQMEM(result_backup, result, history_size * sizeof(uint16_t));
	UT_EXPECT_EQ(0, ret);
}

 #endif
