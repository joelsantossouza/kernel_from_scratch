/*
 * File: test_math.h
 * Author: Joel Souza
 * Date: 2026-04-19
 * Description: Test math library functions and macros
 */

#ifndef TEST_MATH_H
# define TEST_MATH_H

# include "test/unit_test/unit_test.h"
# include "math/math.h"

UT_CREATE_CATEGORY(math, "Test math library")

UT_CREATE_SUITE(math, min, "Test MIN macro")
UT_CREATE_CASE(math, min, left_smaller, "Test with left arg smaller")
{
	UT_EXPECT_EQ(0, MIN(0, 1));
}
UT_CREATE_CASE(math, min, right_smaller, "Test with right arg smaller")
{
	UT_EXPECT_EQ(0, MIN(1, 0));
}
UT_CREATE_CASE(math, min, equal_values, "Test with equal values")
{
	UT_EXPECT_EQ(0, MIN(0, 0));
}
UT_CREATE_CASE(math, min, arg_side_effects, "Test args with side effects")
{
	int	ten;

	ten = 10;
	UT_EXPECT_EQ(10, MIN(ten++, 12));
	ten = 10;
	UT_EXPECT_EQ(9, MIN(--ten, 10));
	ten = 10;
	UT_EXPECT_EQ(10, MIN(12, ten++));
	ten = 10;
	UT_EXPECT_EQ(9, MIN(10, --ten));
}

UT_CREATE_SUITE(math, align_down, "Test align_down function")
UT_CREATE_CASE(math, align_down, unaligned_value, "Test with unaligned value")
{
	UT_EXPECT_EQ(100, align_down(101, 100));
	UT_EXPECT_EQ(100, align_down(199, 100));
}
UT_CREATE_CASE(math, align_down, aligned_value, "Test with aligned value")
{
	UT_EXPECT_EQ(200, align_down(200, 100));
}

UT_CREATE_SUITE(math, align_up, "Test align_up function")
UT_CREATE_CASE(math, align_up, unaligned_value, "Test with unaligned value")
{
	UT_EXPECT_EQ(200, align_up(101, 100));
	UT_EXPECT_EQ(200, align_up(199, 100));
}
UT_CREATE_CASE(math, align_up, aligned_value, "Test with aligned value")
{
	UT_EXPECT_EQ(200, align_up(200, 100));
}

UT_CREATE_SUITE(math, is_pow2, "Test is_pow2 function")
UT_CREATE_CASE(math, is_pow2, true_cases, "Test with power-of-2 numbers")
{
	UT_EXPECT_EQ(true, is_pow2(0x00000001));
	UT_EXPECT_EQ(true, is_pow2(0x00001000));
	UT_EXPECT_EQ(true, is_pow2(0x80000000));
}
UT_CREATE_CASE(math, is_pow2, false_cases, "Test with non-power-of-2 numbers")
{
	UT_EXPECT_EQ(false, is_pow2(0x00000000));
	UT_EXPECT_EQ(false, is_pow2(0x0000F000));
	UT_EXPECT_EQ(false, is_pow2(0xFFFFFFFF));
}

#endif
