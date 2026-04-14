/*
 * File: test_ctype.h
 * Author: Joel Souza
 * Date: 2026-04-14
 * Description: 
 */

#ifndef TEST_CTYPE_H
# define TEST_CTYPE_H

# include "ctype/ctype.h"
# include "test/unit_test.h"

static inline
void	test_ctype_islower_true(void)
{
	UT_EXPECT_EQ(true, islower('a'));
	UT_EXPECT_EQ(true, islower('b'));
	UT_EXPECT_EQ(true, islower('c'));
	UT_EXPECT_EQ(true, islower('d'));
	UT_EXPECT_EQ(true, islower('e'));
	UT_EXPECT_EQ(true, islower('f'));
	UT_EXPECT_EQ(true, islower('g'));
	UT_EXPECT_EQ(true, islower('h'));
	UT_EXPECT_EQ(true, islower('i'));
	UT_EXPECT_EQ(true, islower('j'));
	UT_EXPECT_EQ(true, islower('k'));
	UT_EXPECT_EQ(true, islower('l'));
	UT_EXPECT_EQ(true, islower('m'));
	UT_EXPECT_EQ(true, islower('n'));
	UT_EXPECT_EQ(true, islower('o'));
	UT_EXPECT_EQ(true, islower('p'));
	UT_EXPECT_EQ(true, islower('q'));
	UT_EXPECT_EQ(true, islower('r'));
	UT_EXPECT_EQ(true, islower('s'));
	UT_EXPECT_EQ(true, islower('t'));
	UT_EXPECT_EQ(true, islower('u'));
	UT_EXPECT_EQ(true, islower('v'));
	UT_EXPECT_EQ(true, islower('w'));
	UT_EXPECT_EQ(true, islower('x'));
	UT_EXPECT_EQ(true, islower('y'));
	UT_EXPECT_EQ(true, islower('z'));
}

static inline
void	test_ctype_islower(void)
{
	test_ctype_islower_true();
}

#endif
