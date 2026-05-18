/*
 * File: test_video_text_history.h
 * Author: Joel Souza
 * Date: 2026-05-08
 * Description: Test video text history management functions
 */

#ifndef TEST_VIDEO_TEXT_HISTORY_H
# define TEST_VIDEO_TEXT_HISTORY_H

# include "test/unit_test/unit_test.h"

UT_CREATE_CATEGORY(video_text_history, "Test Video Text History functionalities")
t_video_text_history	history_test;

# define SRC_PATTERN	"This is a Test!"
# define SRC_ATTR		'.'
enum {SRC_SIZE = VIDEO_TEXT_HISTORY_MAX * 2};

char					src_raw[SRC_SIZE];
uint16_t				src_attributed[SRC_SIZE];
uint16_t				result[VIDEO_TEXT_HISTORY_MAX];

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

UT_CREATE_SUITE(video_text_history, write, "Test history write functionality")
UT_CREATE_CASE(video_text_history, write, no_wrap, "Writing data without history wrap")
{
	const uint32_t	nbytes = VIDEO_TEXT_HISTORY_MAX * 0.01;

	history_test.offset = 0;
	UT_LOG_CALL(video_text_history_write(&history_test, src_raw, nbytes, SRC_ATTR));
	UT_EXPECT_EQMEM(src_attributed, history_test.data, nbytes * sizeof(uint16_t));
}
UT_CREATE_CASE(video_text_history, write, with_wrap, "Writing data with history wrap")
{
	const uint32_t	total_nbytes = VIDEO_TEXT_HISTORY_MAX * 0.01;
	const uint32_t	chunk1_nbytes = total_nbytes / 2;
	const uint32_t	chunk2_nbytes = total_nbytes - chunk1_nbytes;
	const uint32_t	offset = VIDEO_TEXT_HISTORY_MAX - chunk1_nbytes;

	history_test.offset = offset;
	UT_LOG_CALL(video_text_history_write(&history_test, src_raw, total_nbytes, SRC_ATTR));
	UT_LOG_CALL(memcpy(result, &history_test.data[offset], chunk1_nbytes * sizeof(uint16_t)));
	UT_LOG_CALL(memcpy(&result[chunk1_nbytes], history_test.data, chunk2_nbytes * sizeof(uint16_t)));
	UT_EXPECT_EQMEM(src_attributed, result, total_nbytes * sizeof(uint16_t));
}
UT_CREATE_CASE(video_text_history, write, overwrite_oldest, "Test history overwrite by passing nbytes > VIDEO_TEXT_HISTORY_MAX")
{
	const uint32_t	overwrite_nbytes = 17;
	const uint32_t	oversized_nbytes = VIDEO_TEXT_HISTORY_MAX + overwrite_nbytes;
	const uint32_t	remaining_nbytes = VIDEO_TEXT_HISTORY_MAX - overwrite_nbytes;

	history_test.offset = 0;
	UT_LOG_CALL(video_text_history_write(&history_test, src_raw, oversized_nbytes, SRC_ATTR));
	UT_LOG_CALL(memcpy(result, &history_test.data[overwrite_nbytes], remaining_nbytes * sizeof(uint16_t)));
	UT_LOG_CALL(memcpy(&result[remaining_nbytes], history_test.data, overwrite_nbytes * sizeof(uint16_t)));
	UT_EXPECT_EQMEM(&src_attributed[overwrite_nbytes], result, VIDEO_TEXT_HISTORY_MAX * sizeof(uint16_t));
}

 #endif
