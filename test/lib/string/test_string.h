/*
 * File: test_string.h
 * Author: Joel Souza
 * Date: 2026-04-20
 * Description: Test string library functions
 */

#ifndef TEST_STRING_H
# define TEST_STRING_H

# include "test/unit_test/unit_test.h"

UT_CREATE_CATEGORY(string, "Test string library")

UT_CREATE_SUITE(string, memcpy, "Test memcpy function")
UT_CREATE_CASE(string, memcpy, aligned_memory_aligned_nbytes, "Test with memory area and nbytes aligned to 32-bit chunk")
{
	char	aligned_buf[4096] __attribute__((aligned(4)));
	char	aligned_src_4B[4] __attribute__((aligned(4))) = {
		[0 ... sizeof(aligned_src_4B) - 2] = 1, [sizeof(aligned_src_4B) - 1] = 0
	};
	char	aligned_src_4KB[4096] __attribute__((aligned(4))) = {
		[0 ... sizeof(aligned_src_4KB) - 2] = 1, [sizeof(aligned_src_4KB) - 1] = 0
	};

	UT_EXPECT_EQ(aligned_src_4B, memcpy(aligned_buf, aligned_src_4B, 4));
	UT_EXPECT_EQ(aligned_src_4KB, memcpy(aligned_buf, aligned_src_4KB, 4096));
}

#endif
