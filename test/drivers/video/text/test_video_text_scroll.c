/*
 * File: test_video_text_scroll.c
 * Author: Joel Souza
 * Date: 2026-09-28
 * Description: Test video text scroll management functions
 */

#include "test/unit_test/unit_test.h"

UT_CREATE_CATEGORY(video_text_scroll, "Test Video Text Scroll functionalities")

/*
 * TODO:
 * */
UT_CREATE_SUITE(video_text_scroll, up, "Test scroll up functionality")
UT_CREATE_CASE(video_text_scroll, up, history_size_le_screensize, "Scrolling up with history size less than or equal to screen size")
{}
UT_CREATE_CASE(video_text_scroll, up, history_size_gt_screensize, "Scrolling up with history size greater than screen size")
{}
UT_CREATE_CASE(video_text_scroll, up, nscroll_gt_max_nscroll, "Scrolling up more than g_video_text_history.nlines - g_video_text_config.height")
{}
UT_CREATE_CASE(video_text_scroll, up, history_lineoffset_eq_0, "Scrolling up with history line offset equal to 0")
{}
UT_CREATE_CASE(video_text_scroll, up, history_lineoffset_neq_0, "Scrolling up with history line offset not equal to 0")
{}

UT_CREATE_SUITE(video_text_scroll, down, "Test scroll down functionality")
