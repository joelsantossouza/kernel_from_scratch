/*
 * File: test_string.c
 * Author: Joel Souza
 * Date: 2026-07-30
 * Description: Test string library functions
 */

# include "test/unit_test/unit_test.h"
# include "test/lib/string/test_string.h"
# include "cpu/flags.h"
# include "errno.h"

UT_CREATE_CATEGORY(string, "Test string library")

static char					aligned_src[SRC_SIZE] __attribute__((aligned(ALIGN_32)));
static char					aligned_dst[SRC_SIZE] __attribute__((aligned(ALIGN_32)));

static char					unaligned_src_raw[SRC_SIZE + 1] __attribute__((aligned(ALIGN_32)));
static char					unaligned_dst_raw[SRC_SIZE + 1] __attribute__((aligned(ALIGN_32)));
static char					*unaligned_src = unaligned_src_raw + 1;
static char					*unaligned_dst = unaligned_dst_raw + 1;

static uint16_t				attributed_src[SRC_SIZE];
static uint16_t				uniform_words_src[SRC_SIZE] = {
	[0 ... SRC_SIZE - 1] = SRC_UNIFORM_WORD
};
static char					uniform_bytes_src[SRC_SIZE] = {
	[0 ... SRC_SIZE - 1] = SRC_UNIFORM_BYTE
};

static char					expected_result[SRC_SIZE];

static const uint32_t		SRC_SIZE_1_PERCENT = SRC_SIZE * 0.01;

__attribute__((constructor))
static void	test_string_src_init(void)
{
	memset_pattern(aligned_src, SRC_PATTERN, SRC_PATTERN_SIZE, SRC_SIZE);
	memset_pattern(unaligned_src, SRC_PATTERN, SRC_PATTERN_SIZE, SRC_SIZE);
	for (int i = 0; i < SRC_SIZE; i++)
		attributed_src[i] = (SRC_ATTR << 8) | aligned_src[i];
}

/*
 * void	*memcpy(void *dest, const void *src, uint32_t n);
 * 
 * Function to copy n bytes from src into dst, then return a pointer to
 * dest
 * */
UT_CREATE_SUITE(string, memcpy, "Test memcpy function")
UT_CREATE_CASE(string, memcpy, return_val_check, "Test if the return address match the dest's address")
{
	UT_EXPECT_EQ(aligned_dst, memcpy(aligned_dst, aligned_src, 0));
	UT_EXPECT_EQ(aligned_dst, memcpy(aligned_dst, aligned_src, 8));
}
UT_CREATE_CASE(string, memcpy, nothing_to_copy, "Test with nbytes == 0, then nothing must be copied")
{
	const uint32_t	nbytes_to_compare = SRC_SIZE_1_PERCENT;

	memset(aligned_dst, 0, nbytes_to_compare);
	UT_LOG_CALL(memset(expected_result, 0, nbytes_to_compare));
	UT_EXPECT_EQMEM(expected_result, memcpy(aligned_dst, aligned_src, 0), nbytes_to_compare);
}
UT_CREATE_CASE(string, memcpy, direction_flag, "Set direction flag, and must still copying forward")
{
	const uint32_t	nbytes = SRC_SIZE_1_PERCENT;

	memset(aligned_dst, 0, nbytes);
	__asm__("std");
	UT_EXPECT_EQMEM(aligned_src, memcpy(aligned_dst, aligned_src, nbytes), nbytes);
}
UT_CREATE_CASE(string, memcpy, aligned_memory, "Test with memory aligned to 32-bit chunk")
{
	const uint32_t	nbytes = SRC_SIZE_1_PERCENT;

	memset(aligned_dst, 0, nbytes);
	UT_EXPECT_EQMEM(aligned_src, memcpy(aligned_dst, aligned_src, nbytes), nbytes);
}
UT_CREATE_CASE(string, memcpy, unaligned_memory, "Test with memory unaligned to 32-bit chunk")
{
	const uint32_t	nbytes = SRC_SIZE_1_PERCENT;

	memset(unaligned_dst, 0, nbytes);
	UT_EXPECT_EQMEM(unaligned_src, memcpy(unaligned_dst, unaligned_src, nbytes), nbytes);
}

/*
 * void	*mempcpy(void *dest, const void *src, uint32_t n);
 *
 * Same as memcpy, but the only difference is that it retuns a
 * pointer to dest + n instead.
 * */
UT_CREATE_SUITE(string, mempcpy, "Test mempcpy function")
UT_CREATE_CASE(string, mempcpy, return_val_check, "Test if the return address match dest + nbytes")
{
	UT_EXPECT_EQ(aligned_dst, mempcpy(aligned_dst, aligned_src, 0));
	UT_EXPECT_EQ(aligned_dst + 8, mempcpy(aligned_dst, aligned_src, 8));
}

/*
 * void	*memmove(void *dest, const void *src, uint32_t n);
 *
 * Same as memcpy, with the differential of solving the memcpy's
 * overlap memory limitation
 * */
UT_CREATE_SUITE(string, memmove, "Test memmove, function")
UT_CREATE_CASE(string, memmove, return_val_check, "Test if the return address match the dest's address")
{
	const uint32_t	nbytes = SRC_SIZE_1_PERCENT;
	const uint32_t	noverlap = nbytes * 0.10;

	UT_EXPECT_EQ(aligned_dst, memmove(aligned_dst, aligned_dst + noverlap, nbytes));
	UT_EXPECT_EQ(aligned_dst + noverlap, memmove(aligned_dst + noverlap, aligned_dst, nbytes));
}
UT_CREATE_CASE(string, memmove, overlap_nothing_to_copy, "Test overlap with nbytes == 0, then nothing must be copied")
{
	const uint32_t	nbytes_to_compare = SRC_SIZE_1_PERCENT;
	const uint32_t	noverlap = nbytes_to_compare * 0.10;

	memcpy(aligned_dst, aligned_src, nbytes_to_compare);
	UT_LOG_CALL(memcpy(expected_result, aligned_dst, nbytes_to_compare));
	UT_LOG_CALL(memmove(aligned_dst + noverlap, aligned_dst, 0));
	UT_EXPECT_EQMEM(expected_result, aligned_dst, nbytes_to_compare);
}
UT_CREATE_CASE(string, memmove, is_direction_flag_clean, "Test if direction flag is clean after call")
{
	const uint32_t	nbytes = SRC_SIZE_1_PERCENT;
	const uint32_t	noverlap = nbytes * 0.10;

	UT_LOG_CALL(memmove(aligned_dst + noverlap, aligned_dst, nbytes));
	UT_EXPECT_EQ(0, cpu_flags_get() & CPU_FLAGS_DF);
}
UT_CREATE_CASE(string, memmove, overlap_aligned_memory, "Test overlap with memory aligned to 32-bit chunk")
{
	const uint32_t	noverlap = 8;
	const uint32_t	nbytes = SRC_SIZE - noverlap;

	UT_LOG_CALL(memcpy(aligned_dst, aligned_src, nbytes));
	UT_EXPECT_EQMEM(aligned_src, memmove(aligned_dst + noverlap, aligned_dst, nbytes), nbytes);
}
UT_CREATE_CASE(string, memmove, overlap_unaligned_memory, "Test overlap with memory unaligned to 32-bit chunk")
{
	const uint32_t	noverlap = 7;
	const uint32_t	nbytes = SRC_SIZE - noverlap;

	UT_LOG_CALL(memcpy(unaligned_dst, unaligned_src, nbytes));
	UT_EXPECT_EQMEM(unaligned_src, memmove(unaligned_dst + noverlap, unaligned_dst, nbytes), nbytes);
}

/*
 * void	*memicpy8(void *dest, const void *src, uint32_t n, uint8_t attr);
 *
 * Function to copy n bytes of src into dst, interleaving byte-per-byte
 * with attr
 * */
UT_CREATE_SUITE(string, memicpy8, "Test memicpy8 function")
UT_CREATE_CASE(string, memicpy8, return_val_check, "Test if the return addresss match dest's address")
{
	UT_EXPECT_EQ(aligned_dst, memicpy8(aligned_dst, aligned_src, 0, 0xFF));
	UT_EXPECT_EQ(aligned_dst, memicpy8(aligned_dst, aligned_src, 8, 0xFF));
}
UT_CREATE_CASE(string, memicpy8, nothing_to_copy, "Test with nbytes == 0, then nothing must be copied")
{
	const uint32_t	nbytes_to_compare = SRC_SIZE_1_PERCENT;

	memset(aligned_dst, 0, nbytes_to_compare);
	UT_LOG_CALL(memcpy(expected_result, aligned_dst, nbytes_to_compare));
	UT_EXPECT_EQMEM(expected_result, memicpy8(aligned_dst, aligned_src, 0, 0xFF), nbytes_to_compare);
}
UT_CREATE_CASE(string, memicpy8, aligned_memory, "Test with memory aligned to 32-bit chunk")
{
	const uint32_t	nwords = SRC_SIZE_1_PERCENT;
	const uint32_t	nbytes = nwords * sizeof(uint16_t);

	memset(aligned_dst, 0, nbytes);
	UT_EXPECT_EQMEM(attributed_src, memicpy8(aligned_dst, aligned_src, nwords, SRC_ATTR), nbytes);
}
UT_CREATE_CASE(string, memicpy8, unaligned_memory, "Test with memory unaligned to 32-bit chunk")
{
	const uint32_t	nwords = SRC_SIZE_1_PERCENT;
	const uint32_t	nbytes = nwords * sizeof(uint16_t);

	memset(unaligned_dst, 0, nbytes);
	UT_EXPECT_EQMEM(attributed_src, memicpy8(unaligned_dst, unaligned_src, nwords, SRC_ATTR), nbytes);
}

/*
 * int	memcmp(const void *s1, const void *s2, uint32_t n);
 *
 * Function to compare n bytes of s1 and s2 memory areas
 * */
UT_CREATE_SUITE(string, memcmp, "Test memcmp function")
static const char	aligned_src_eq[17] __attribute__((aligned(ALIGN_32))) = "|..............|";
static const char	aligned_src_lt[17] __attribute__((aligned(ALIGN_32))) = "|.............A|";
static const char	aligned_src_gt[17] __attribute__((aligned(ALIGN_32))) = "|.............Z|";

static const char	*unaligned_src_eq = aligned_src_eq + 1;
static const char	*unaligned_src_lt = aligned_src_lt + 1;
static const char	*unaligned_src_gt = aligned_src_gt + 1;

UT_CREATE_CASE(string, memcmp, nothing_to_compare, "Test with nbytes == 0, then should always return 0")
{
	UT_EXPECT_EQ(0, memcmp("abc", "abc", 0));
	UT_EXPECT_EQ(0, memcmp("abc", "abz", 0));
}
UT_CREATE_CASE(string, memcmp, direction_flag, "Set direction flag, and must still comparing forward")
{
	/* This test case verifies whether the `memcmp()` function clears the DF (Direction Flag)
	 * using a string that is half different and half identical. Since the equality begins in the middle,
	 * a true result indicates that the comparison was performed in the forward direction;
	 * otherwise, the result will be false. */

	__asm__("std");
	UT_EXPECT_EQ(0, memcmp("diff_equal" + 4, "????_equal" + 4, 6));
}
UT_CREATE_CASE(string, memcmp, small_comparison, "Test with 0 < nbytes < 32-bit chunk")
{
	UT_EXPECT_LT(0, memcmp("abc", "abz", 3));
	UT_EXPECT_EQ(0, memcmp("abc", "abc", 3));
	UT_EXPECT_GT(0, memcmp("abz", "abc", 3));
}
UT_CREATE_CASE(string, memcmp, difference_inside_chunk, "Test with difference inside a 32-bit chunk")
{
	UT_EXPECT_LT(0, memcmp("..A.", "..Z.", 4));
	UT_EXPECT_EQ(0, memcmp("..A.", "..A.", 4));
	UT_EXPECT_GT(0, memcmp("..Z.", "..A.", 4));
}
UT_CREATE_CASE(string, memcmp, aligned_memory, "Test comparisons with memory aligned to 32-bit chunk")
{
	UT_EXPECT_LT(0, memcmp(aligned_src_lt, aligned_src_gt, 16));
	UT_EXPECT_EQ(0, memcmp(aligned_src_eq, aligned_src_eq, 16));
	UT_EXPECT_GT(0, memcmp(aligned_src_gt, aligned_src_lt, 16));
}
UT_CREATE_CASE(string, memcmp, unaligned_memory, "Test comparisons with memory unaligned to 32-bit chunk")
{
	UT_EXPECT_LT(0, memcmp(unaligned_src_lt, unaligned_src_gt, 16));
	UT_EXPECT_EQ(0, memcmp(unaligned_src_eq, unaligned_src_eq, 16));
	UT_EXPECT_GT(0, memcmp(unaligned_src_gt, unaligned_src_lt, 16));
}

/*
 * void	*memset(void *dest, int c, uint32_t n);
 *
 * Function to set the first 'n' bytes of 'dest' with the byte 'c'
 * */
UT_CREATE_SUITE(string, memset, "Test memset function")
UT_CREATE_CASE(string, memset, return_val_check, "Test if the return address match the dest's address")
{
	UT_EXPECT_EQ(aligned_dst, memset(aligned_dst, 0, 0));
	UT_EXPECT_EQ(aligned_dst, memset(aligned_dst, 0, 8));
}
UT_CREATE_CASE(string, memset, nothing_to_set, "Test with nbytes == 0, then nothing must be set")
{
	const uint32_t	nbytes_to_compare = SRC_SIZE_1_PERCENT;

	memcpy(aligned_dst, aligned_src, nbytes_to_compare);
	UT_LOG_CALL(memcpy(expected_result, aligned_dst, nbytes_to_compare));
	UT_EXPECT_EQMEM(expected_result, memset(aligned_dst, 1, 0), nbytes_to_compare);
}
UT_CREATE_CASE(string, memset, direction_flag, "Set direction flag, and must still forward memory setting")
{
	const uint32_t	nbytes = SRC_SIZE_1_PERCENT;

	memset(aligned_dst, 0, nbytes);
	__asm__("std");
	UT_EXPECT_EQMEM(uniform_bytes_src, memset(aligned_dst, SRC_UNIFORM_BYTE, nbytes), nbytes);
}
UT_CREATE_CASE(string, memset, aligned_memory, "Test with memory aligned to 32-bit chunk")
{
	const uint32_t	nbytes = SRC_SIZE_1_PERCENT;

	memset(aligned_dst, 0, nbytes);
	UT_EXPECT_EQMEM(uniform_bytes_src, memset(aligned_dst, SRC_UNIFORM_BYTE, nbytes), nbytes);
}
UT_CREATE_CASE(string, memset, unaligned_memory, "Test with memory unaligned to 32-bit chunk")
{
	const uint32_t	nbytes = SRC_SIZE_1_PERCENT;

	memset(unaligned_dst, 0, nbytes);
	UT_EXPECT_EQMEM(uniform_bytes_src, memset(unaligned_dst, SRC_UNIFORM_BYTE, nbytes), nbytes);
}


/*
 * void	*memsetw(void *dest, int c, uint32_t n);
 *
 * Function to set the first 'n' words of 'dest' with the word 'c'
 * */
UT_CREATE_SUITE(string, memsetw, "Test memsetw function")
UT_CREATE_CASE(string, memsetw, return_val_check, "Test if the return address match the dest's address")
{
	UT_EXPECT_EQ(aligned_dst, memsetw(aligned_dst, 0, 0));
	UT_EXPECT_EQ(aligned_dst, memsetw(aligned_dst, 0, 4));
}
UT_CREATE_CASE(string, memsetw, nothing_to_set, "Test with nwords == 0, then nothing must be set")
{
	const uint32_t	nbytes_to_compare = SRC_SIZE_1_PERCENT;

	memcpy(aligned_dst, aligned_src, nbytes_to_compare);
	UT_LOG_CALL(memcpy(expected_result, aligned_dst, nbytes_to_compare));
	UT_EXPECT_EQMEM(expected_result, memsetw(aligned_dst, 1, 0), nbytes_to_compare);
}
UT_CREATE_CASE(string, memsetw, direction_flag, "Set direction flag, and must still forward memory setting")
{
	const uint32_t	nwords = SRC_SIZE_1_PERCENT;
	const uint32_t	nbytes = nwords * sizeof(uint16_t);

	memset(aligned_dst, 0, nbytes);
	__asm__("std");
	UT_EXPECT_EQMEM(uniform_words_src, memsetw(aligned_dst, SRC_UNIFORM_WORD, nwords), nbytes);
}
UT_CREATE_CASE(string, memsetw, aligned_memory, "Test with memory aligned to 32-bit chunk")
{
	const uint32_t	nwords = SRC_SIZE_1_PERCENT;
	const uint32_t	nbytes = nwords * sizeof(uint16_t);

	memset(aligned_dst, 0, nbytes);
	UT_EXPECT_EQMEM(uniform_words_src, memsetw(aligned_dst, SRC_UNIFORM_WORD, nwords), nbytes);
}
UT_CREATE_CASE(string, memsetw, unaligned_memory, "Test with memory unaligned to 32-bit chunk")
{
	const uint32_t	nwords = SRC_SIZE_1_PERCENT;
	const uint32_t	nbytes = nwords * sizeof(uint16_t);

	memset(unaligned_dst, 0, nbytes);
	UT_EXPECT_EQMEM(uniform_words_src, memsetw(unaligned_dst, SRC_UNIFORM_WORD, nwords), nbytes);
}


/*
 * void	*memset_pattern(void *dest, const void *pattern, uint32_t pattern_size, uint32_t n);
 *
 * Set the first 'n' bytes of 'dest' by repeating 'pattern'.
 * If 'n' is not multiple of 'pattern_size', the final repetition
 * is truncated.
 * */
UT_CREATE_SUITE(string, memset_pattern, "Test memset_pattern function")
UT_CREATE_CASE(string, memset_pattern, return_val_check, "Test if the return address match the dest's address")
{
	UT_EXPECT_EQ(aligned_dst, memset_pattern(aligned_dst, "123", 3, 0));
	UT_EXPECT_EQ(aligned_dst, memset_pattern(aligned_dst, "123", 3, 32));
}
UT_CREATE_CASE(string, memset_pattern, nothing_to_set, "Test with nbytes == 0, then nothing must be set")
{
	const uint32_t	nbytes_to_compare = SRC_SIZE_1_PERCENT;

	memset(aligned_dst, 0, nbytes_to_compare);
	UT_LOG_CALL(memcpy(expected_result, aligned_dst, nbytes_to_compare));
	UT_EXPECT_EQMEM(expected_result, memset_pattern(aligned_dst, "123", 3, 0), nbytes_to_compare);
}
UT_CREATE_CASE(string, memset_pattern, empty_pattern, "Test with pattern size == 0, then nothing must be set")
{
	const uint32_t	nbytes_to_compare = SRC_SIZE_1_PERCENT;

	memset(aligned_dst, 0, nbytes_to_compare);
	UT_LOG_CALL(memcpy(expected_result, aligned_dst, nbytes_to_compare));
	UT_EXPECT_EQMEM(expected_result, memset_pattern(aligned_dst, "", 0, nbytes_to_compare), nbytes_to_compare);
}
UT_CREATE_CASE(string, memset_pattern, pattern_greater_than_count, "Test with pattern size exceeding the number of entries to set")
{
	const uint32_t	nbytes = SRC_SIZE_1_PERCENT;

	memset(aligned_dst, 0, nbytes);
	UT_EXPECT_EQMEM(aligned_src, memset_pattern(aligned_dst, aligned_src, SRC_SIZE, nbytes), nbytes);
}
UT_CREATE_CASE(string, memset_pattern, count_greater_than_pattern, "Test with number of entries to set exceeding the pattern size")
{
	const uint32_t	nbytes = SRC_SIZE_1_PERCENT;

	memset(aligned_dst, 0, nbytes);
	UT_EXPECT_EQMEM(aligned_src, memset_pattern(aligned_dst, SRC_PATTERN, SRC_PATTERN_SIZE, nbytes), nbytes);
}

/*
 * int	strcmp(const char *s1, const char *s2);
 *
 * Function to compare the two strings s1 and s2.
 * */
UT_CREATE_SUITE(string, strcmp, "Test strcmp function")
UT_CREATE_CASE(string, strcmp, s1_smaller, "Test with string1 smaller")
{
	UT_EXPECT_LT(0, strcmp("", "abc"));
	UT_EXPECT_LT(0, strcmp("abCd", "abcd"));
}
UT_CREATE_CASE(string, strcmp, equal, "Test with s1 equal to s2")
{
	UT_EXPECT_EQ(0, strcmp("", ""));
	UT_EXPECT_EQ(0, strcmp("abCd", "abCd"));
}
UT_CREATE_CASE(string, strcmp, s1_greater, "Test with string1 greater")
{
	UT_EXPECT_GT(0, strcmp("abc", ""));
	UT_EXPECT_GT(0, strcmp("abcd", "abCd"));
}

/*
 * uint32_t	strlen(const char *s);
 *
 * Function to calculate the length of a string
 * */
UT_CREATE_SUITE(string, strlen, "Test strlen function")
UT_CREATE_CASE(string, strlen, len_0, "Test with strings of length 0")
{
	UT_EXPECT_EQ(0, strlen(""));
}
UT_CREATE_CASE(string, strlen, len_gt_0, "Test with strings of length > 0")
{
	UT_EXPECT_EQ(5, strlen("01234"));
	UT_EXPECT_EQ(10, strlen("0123456789"));
}

/*
 * int strnlen_strict(const char *s, uint32_t n, uint32_t *len);
 *
 * Computes the length of string s up to a maximum of n bytes and stores
 * the result in *len.
 *
 * Returns 0 if the string length is less than or equal to n.
 * Returns -ENAMETOOLONG if the string exceeds n bytes, in which case
 * *len is set to n.
 */
UT_CREATE_SUITE(string, strnlen_strict, "Test strnlen_strict function")
static uint32_t	len;

UT_CREATE_CASE(string, strnlen_strict, return_val_check_valid, "Test with s length <= n, then return val must be 0")
{
	UT_EXPECT_EQ(0, strnlen_strict("", 0, &len));
	UT_EXPECT_EQ(0, strnlen_strict("", 3, &len));
	UT_EXPECT_EQ(0, strnlen_strict("abc", 3, &len));
}
UT_CREATE_CASE(string, strnlen_strict, len_val_valid, "Test length calculation with s length <= n")
{
	UT_LOG_CALL(strnlen_strict("", 0, &len));
	UT_EXPECT_EQ(0, len);
	UT_LOG_CALL(strnlen_strict("abc", 3, &len));
	UT_EXPECT_EQ(3, len);
}
UT_CREATE_CASE(string, strnlen_strict, return_val_check_invalid, "Test with s length > n, then return val must be -ENAMETOOLONG")
{
	UT_EXPECT_EQ(-ENAMETOOLONG, strnlen_strict("a", 0, &len));
	UT_EXPECT_EQ(-ENAMETOOLONG, strnlen_strict("abcd", 3, &len));
}
UT_CREATE_CASE(string, strnlen_strict, len_val_invalid, "Test with s length > n, then length calculation should be equal to n")
{
	UT_LOG_CALL(strnlen_strict("a", 0, &len));
	UT_EXPECT_EQ(0, len);
	UT_LOG_CALL(strnlen_strict("abcd", 3, &len));
	UT_EXPECT_EQ(3, len);
}
