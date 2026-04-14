/*
 * File: test_ctype.c
 * Author: Joel Souza
 * Date: 2026-04-14
 * Description: 
 */

#include "test/unit_test.h"
#include "ctype/ctype.h"

// Test islower()
void	test_ctype_islower_true(void)
{
	UT_EXPECT_EQ(true, islower('a'));
}

void	test_ctype_islower(void)
{
	test_ctype_islower_true();
}
