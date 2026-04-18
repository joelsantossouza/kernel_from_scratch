/*
 * File: test_ctype.h
 * Author: Joel Souza
 * Date: 2026-04-16
 * Description: Test ctype library functions
 */

#ifndef TEST_CTYPE_H
# define TEST_CTYPE_H

# include "ctype/ctype.h"
# include "test/unit_test/unit_test.h"

UT_CREATE_CATEGORY(ctype, "Test ctype library")

UT_CREATE_SUITE(ctype, islower, "Test islower function")
UT_CREATE_CASE(ctype, islower, true_cases, "Test with lower chars")
{
	UT_EXPECT_EQ(true, islower('a'));
	UT_EXPECT_EQ(true, islower('n'));
	UT_EXPECT_EQ(true, islower('z'));
}
UT_CREATE_CASE(ctype, islower, false_cases, "Test with non-lower chars")
{
	UT_EXPECT_EQ(false, islower('a' - 1));
	UT_EXPECT_EQ(false, islower('z' + 1));
}

UT_CREATE_SUITE(ctype, isprint, "Test isprint function")
UT_CREATE_CASE(ctype, isprint, true_cases, "Test with printable chars")
{
	UT_EXPECT_EQ(true, isprint(' '));
	UT_EXPECT_EQ(true, isprint('A'));
	UT_EXPECT_EQ(true, isprint('~'));
}
UT_CREATE_CASE(ctype, isprint, false_cases, "Test with non-printable chars")
{
	UT_EXPECT_EQ(false, isprint(' ' - 1));
	UT_EXPECT_EQ(false, isprint('~' + 1));
}

UT_CREATE_SUITE(ctype, iscntrl, "Test iscntrl function")
UT_CREATE_CASE(ctype, iscntrl, true_cases, "Test with control-characters")
{
	UT_EXPECT_EQ(true, iscntrl('\0'));
	UT_EXPECT_EQ(true, iscntrl(0x1F));
	UT_EXPECT_EQ(true, iscntrl(0x7F));
}
UT_CREATE_CASE(ctype, iscntrl, false_cases, "Test with non-control-characters")
{
	UT_EXPECT_EQ(false, iscntrl('\0' - 1));
	UT_EXPECT_EQ(false, iscntrl(0x1F + 1));
	UT_EXPECT_EQ(false, iscntrl(0x7F - 1));
	UT_EXPECT_EQ(false, iscntrl(0x7F + 1));
}

UT_CREATE_SUITE(ctype, toupper, "Test toupper function")
UT_CREATE_CASE(ctype, toupper, lower_chars, "Test with lower chars")
{
	UT_EXPECT_EQ('A', toupper('a'));
	UT_EXPECT_EQ('N', toupper('n'));
	UT_EXPECT_EQ('Z', toupper('z'));
}
UT_CREATE_CASE(ctype, toupper, non_lower_chars, "Test with non-lower chars")
{
	UT_EXPECT_EQ('a' - 1, toupper('a' - 1));
	UT_EXPECT_EQ('z' + 1, toupper('z' + 1));
}

#endif
