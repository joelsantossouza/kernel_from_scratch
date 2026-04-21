/*
 * File: test_string.h
 * Author: Joel Souza
 * Date: 2026-04-20
 * Description: Test string library functions
 */

#ifndef TEST_STRING_H
# define TEST_STRING_H

# include "test/unit_test/unit_test.h"

const char	aligned_src1[4096] __attribute__((aligned(4))) = {
	[0 ... sizeof(aligned_src1) - 1] = 42
};
const char	aligned_src2[4096] __attribute__((aligned(4))) = {
	[0 ... sizeof(aligned_src2) - 1] = 24
};
const char	*unaligned_src1 = aligned_src1 + 1;
const char	*unaligned_src2 = aligned_src2 + 1;
const char	*src1 = aligned_src1;
const char	*src2 = aligned_src2;

char		aligned_dst[4096] __attribute__((aligned(4)));
char		*unaligned_dst = aligned_dst + 2;
char		*dst = aligned_dst;

UT_CREATE_CATEGORY(string, "Test string library")

UT_CREATE_SUITE(string, memcpy, "Test memcpy function")
UT_CREATE_CASE(string, memcpy, return_val_check, "Test if the return address match the dest's address")
{
	UT_EXPECT_EQ(dst, memcpy(dst, src1, 4));
}
UT_CREATE_CASE(string, memcpy, nothing_to_copy, "Test with nbytes == 0, then nothing must be copied")
{
	char	dst_backup[4] = {0};

	memcpy(dst, dst_backup, 4);
	UT_EXPECT_MEMEQ(dst_backup, memcpy(dst, src2, 0), 4);
}
UT_CREATE_CASE(string, memcpy, aligned_memory, "Test with memory aligned to 32-bit chunk")
{
	UT_EXPECT_MEMEQ(aligned_src1, memcpy(aligned_dst, aligned_src1, 4), 4);
	UT_EXPECT_MEMEQ(aligned_src1, memcpy(aligned_dst, aligned_src1, 4095), 4095);
}
UT_CREATE_CASE(string, memcpy, unaligned_memory, "Test with memory unaligned to 32-bit chunk")
{
	UT_EXPECT_MEMEQ(unaligned_src2, memcpy(unaligned_dst, unaligned_src2, 3), 3);
	UT_EXPECT_MEMEQ(unaligned_src2, memcpy(unaligned_dst, unaligned_src2, 4094), 4094);
}

UT_CREATE_SUITE(string, mempcpy, "Test mempcpy function")
UT_CREATE_CASE(string, mempcpy, return_val_check, "Test if the return address match dest + nbytes")
{
	UT_EXPECT_EQ(dst + 4, mempcpy(dst, src1, 4));
}

UT_CREATE_SUITE(string, memmove, "Test memmove, function")
UT_CREATE_CASE(string, memmove, return_val_check, "Test if the return address match the dest's address")
{
	UT_EXPECT_EQ(dst, memmove(dst, dst + 4, 8));
	UT_EXPECT_EQ(dst + 4, memmove(dst + 4, dst, 8));
}
//UT_CREATE_CASE(string, memmove, overlap_aligned_memory, "Test overlap with memory aligned to 32-bit chunk")
//{
//	UT_EXPECT_MEMEQ()
//}

#endif
