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
const char	aligned_src1[4096] __attribute__((aligned(4))) = {
	[0 ... 2047] = 24, [2048 ... sizeof(aligned_src1) - 1] = 42
};
const char	aligned_src2[4096] __attribute__((aligned(4))) = {
	[0 ... 2047] = 42, [2048 ... sizeof(aligned_src2) - 1] = 24
};
const char	*unaligned_src1 = aligned_src1 + 1;
const char	*unaligned_src2 = aligned_src2 + 1;
const char	*src1 = aligned_src1;
const char	*src2 = aligned_src2;

char		aligned_dst[4096] __attribute__((aligned(4)));
char		*unaligned_dst = aligned_dst + 2;
char		*dst = aligned_dst;

/*
 * void *memcpy(void *dst, void *src, uint32_t n)
 * 
 * Function to copy n bytes from src into dst, then return a pointer to
 * dest
 * */
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

/*
 * void *mempcpy(void *dst, void *src, uint32_t n)
 * 
 * Same as memcpy, but the only difference is that it retuns a
 * pointer to dest + n instead.
 * */
UT_CREATE_SUITE(string, mempcpy, "Test mempcpy function")
UT_CREATE_CASE(string, mempcpy, return_val_check, "Test if the return address match dest + nbytes")
{
	UT_EXPECT_EQ(dst + 4, mempcpy(dst, src1, 4));
}

/*
 * void *memmove(void *dst, void *src, uint32_t n)
 *
 * Same as memcpy, with the differential of solving the memcpy's
 * overlap memory limitation
 * */
UT_CREATE_SUITE(string, memmove, "Test memmove, function")
UT_CREATE_CASE(string, memmove, return_val_check, "Test if the return address match the dest's address")
{
	UT_EXPECT_EQ(dst, memmove(dst, dst + 4, 8));
	UT_EXPECT_EQ(dst + 4, memmove(dst + 4, dst, 8));
}
UT_CREATE_CASE(string, memmove, overlap_nothing_to_copy, "Test overlap with nbytes == 0, then nothing must be copied")
{
	char	dst_backup[4] = {
		[0 ... 1] = 24, [2 ... 3] = 42
	};

	memset(dst, 0, 4);
	memcpy(dst + 4, dst_backup, 4);
	UT_EXPECT_MEMEQ(dst_backup, memmove(dst + 4, dst, 0), 4);
}
UT_CREATE_CASE(string, memmove, overlap_aligned_memory, "Test overlap with memory aligned to 32-bit chunk")
{
	const char	*src_backup = aligned_src2;

	memcpy(aligned_dst, src_backup, 4089);
	UT_EXPECT_MEMEQ(src_backup, memmove(aligned_dst + 4, aligned_dst, 4089), 4089);
}
UT_CREATE_CASE(string, memmove, overlap_unaligned_memory, "Test overlap with memory unaligned to 32-bit chunk")
{
	const char	*src_backup = aligned_src1;

	memcpy(unaligned_dst, src_backup, 4089);
	UT_EXPECT_MEMEQ(src_backup, memmove(unaligned_dst + 4, unaligned_dst, 4089), 4089);
}

/*
 * void *memicpy8(void *dst, void *src, uint32_t n, uint8_t attr)
 *
 * Function to copy n bytes of src into dst, interleaving byte-per-byte
 * with attr
 * */
UT_CREATE_SUITE(string, memicpy8, "Test memicpy8 function")
const char	interleaved_src1[16] = {
	24, '.', 24, '.', 24, '.', 24, '.', 24, '.', 24, '.', 24, '.', 24, '.', 
};
const char	interleaved_src2[16] = {
	42, '.', 42, '.', 42, '.', 42, '.', 42, '.', 42, '.', 42, '.', 42, '.', 
};

UT_CREATE_CASE(string, memicpy8, return_val_check, "Test if the return addresss match dest's address")
{
	UT_EXPECT_EQ(dst, memicpy8(dst, src2, 4, 0xFF));
}
UT_CREATE_CASE(string, memicpy8, nothing_to_copy, "Test with nbytes == 0, then nothing must be copied")
{
	char	dst_backup[8] = {0};

	memcpy(dst, dst_backup, 8);
	UT_EXPECT_MEMEQ(dst_backup, memicpy8(dst, src1, 0, 0xFF), 8);
}
UT_CREATE_CASE(string, memicpy8, aligned_memory, "Test with memory aligned to 32-bit chunk")
{
	UT_EXPECT_MEMEQ(interleaved_src1, memicpy8(aligned_dst, aligned_src1, 7, '.'), 14);
}
UT_CREATE_CASE(string, memicpy8, unaligned_memory, "Test with memory unaligned to 32-bit chunk")
{
	UT_EXPECT_MEMEQ(interleaved_src2, memicpy8(unaligned_dst, unaligned_src2, 8, '.'), 16);
}

/*
 * int	memcmp(void *s1, void *s2, uint32_t n)
 *
 * Function to compare n bytes of s1 and s2 memory areas
 * */
UT_CREATE_SUITE(string, memcmp, "Test memcmp function")
const char	*aligned_src_lt = aligned_src1;
const char	*aligned_src_gt = aligned_src1 + 4;
const char	*unaligned_src_gt = aligned_src2 + 1;
const char	*unaligned_src_lt = aligned_src2 + 5;

UT_CREATE_CASE(string, memcmp, nothing_to_compare, "Test with nbytes == 0, then should always return 0")
{
	UT_EXPECT_EQ(0, memcmp(src1, src2, 0));
}
UT_CREATE_CASE(string, memcmp, small_comparison, "Test with 0 < nbytes < 32-bit chunk")
{
	UT_EXPECT_LT(0, memcmp("abc", "abz", 3));
	UT_EXPECT_EQ(0, memcmp("abc", "abc", 3));
	UT_EXPECT_GT(0, memcmp("abz", "abc", 3));
}
UT_CREATE_CASE(string, memcmp, aligned_memory, "Test comparisons with memory aligned to 32-bit chunk")
{
	UT_EXPECT_LT(0, memcmp(aligned_src_lt, aligned_src_gt, 2048));
	UT_EXPECT_EQ(0, memcmp(aligned_src1, aligned_src1, 2048));
	UT_EXPECT_GT(0, memcmp(aligned_src_gt, aligned_src_lt, 2048));
}
UT_CREATE_CASE(string, memcmp, unaligned_memory, "Test comparisons with memory unaligned to 32-bit chunk")
{
	UT_EXPECT_LT(0, memcmp(unaligned_src_lt, unaligned_src_gt, 2048));
	UT_EXPECT_EQ(0, memcmp(unaligned_src1, unaligned_src1, 2048));
	UT_EXPECT_GT(0, memcmp(unaligned_src_gt, unaligned_src_lt, 2048));
}

#endif
