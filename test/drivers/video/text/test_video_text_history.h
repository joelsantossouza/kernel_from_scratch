/*
 * File: test_video_text_history.h
 * Author: Joel Souza
 * Date: 2026-05-08
 * Description: Test video text history management functions
 */

#ifndef TEST_VIDEO_TEXT_HISTORY_H
# define TEST_VIDEO_TEXT_HISTORY_H

# include "test/unit_test/unit_test.h"
# include "drivers/video/text/video_text.h"

# undef SRC_PATTERN
# define SRC_PATTERN		"Hello from EdenOS!"

# undef SRC_ATTR
# define SRC_ATTR			'.'

# undef SRC_UNIFORM_WORD
# define SRC_UNIFORM_WORD	0xC0FE

# undef SRC_SIZE
enum {SRC_SIZE = VIDEO_TEXT_HISTORY_MAX * 2};

extern UT_DECLARE_CATEGORY(video_text_history);

 #endif
