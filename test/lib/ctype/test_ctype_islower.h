/*
 * File: test_ctype_islower.h
 * Author: Joel Souza
 * Date: 2026-04-14
 * Description: islower() test of true, falses, invalid cases
 */

#ifndef TEST_CTYPE_ISLOWER_H
# define TEST_CTYPE_ISLOWER_H

# include "ctype/ctype.h"
# include "test/unit_test/unit_test.h"

# ifndef TEST_CTYPE_H
#  define TEST_CTYPE_H
UT_CREATE_CATEGORY(ctype, "Test ctype library");
# endif

UT_CREATE_SUITE(ctype, islower, "Test islower function")

UT_CREATE_CASE(ctype, islower, true_cases, "Lower characters case")
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

UT_CREATE_CASE(ctype, islower, false_cases, "Non lower characters case")
{
	UT_EXPECT_EQ(false, islower(0));
	UT_EXPECT_EQ(false, islower(1));
	UT_EXPECT_EQ(false, islower(2));
	UT_EXPECT_EQ(false, islower(3));
	UT_EXPECT_EQ(false, islower(4));
	UT_EXPECT_EQ(false, islower(5));
	UT_EXPECT_EQ(false, islower(6));
	UT_EXPECT_EQ(false, islower(7));
	UT_EXPECT_EQ(false, islower(8));
	UT_EXPECT_EQ(false, islower(9));
	UT_EXPECT_EQ(false, islower(10));
	UT_EXPECT_EQ(false, islower(11));
	UT_EXPECT_EQ(false, islower(12));
	UT_EXPECT_EQ(false, islower(13));
	UT_EXPECT_EQ(false, islower(14));
	UT_EXPECT_EQ(false, islower(15));
	UT_EXPECT_EQ(false, islower(16));
	UT_EXPECT_EQ(false, islower(17));
	UT_EXPECT_EQ(false, islower(18));
	UT_EXPECT_EQ(false, islower(19));
	UT_EXPECT_EQ(false, islower(20));
	UT_EXPECT_EQ(false, islower(21));
	UT_EXPECT_EQ(false, islower(22));
	UT_EXPECT_EQ(false, islower(23));
	UT_EXPECT_EQ(false, islower(24));
	UT_EXPECT_EQ(false, islower(25));
	UT_EXPECT_EQ(false, islower(26));
	UT_EXPECT_EQ(false, islower(27));
	UT_EXPECT_EQ(false, islower(28));
	UT_EXPECT_EQ(false, islower(29));
	UT_EXPECT_EQ(false, islower(30));
	UT_EXPECT_EQ(false, islower(31));
	UT_EXPECT_EQ(false, islower(32));
	UT_EXPECT_EQ(false, islower(33));
	UT_EXPECT_EQ(false, islower(34));
	UT_EXPECT_EQ(false, islower(35));
	UT_EXPECT_EQ(false, islower(36));
	UT_EXPECT_EQ(false, islower(37));
	UT_EXPECT_EQ(false, islower(38));
	UT_EXPECT_EQ(false, islower(39));
	UT_EXPECT_EQ(false, islower(40));
	UT_EXPECT_EQ(false, islower(41));
	UT_EXPECT_EQ(false, islower(42));
	UT_EXPECT_EQ(false, islower(43));
	UT_EXPECT_EQ(false, islower(44));
	UT_EXPECT_EQ(false, islower(45));
	UT_EXPECT_EQ(false, islower(46));
	UT_EXPECT_EQ(false, islower(47));
	UT_EXPECT_EQ(false, islower(48));
	UT_EXPECT_EQ(false, islower(49));
	UT_EXPECT_EQ(false, islower(50));
	UT_EXPECT_EQ(false, islower(51));
	UT_EXPECT_EQ(false, islower(52));
	UT_EXPECT_EQ(false, islower(53));
	UT_EXPECT_EQ(false, islower(54));
	UT_EXPECT_EQ(false, islower(55));
	UT_EXPECT_EQ(false, islower(56));
	UT_EXPECT_EQ(false, islower(57));
	UT_EXPECT_EQ(false, islower(58));
	UT_EXPECT_EQ(false, islower(59));
	UT_EXPECT_EQ(false, islower(60));
	UT_EXPECT_EQ(false, islower(61));
	UT_EXPECT_EQ(false, islower(62));
	UT_EXPECT_EQ(false, islower(63));
	UT_EXPECT_EQ(false, islower(64));
	UT_EXPECT_EQ(false, islower(65));
	UT_EXPECT_EQ(false, islower(66));
	UT_EXPECT_EQ(false, islower(67));
	UT_EXPECT_EQ(false, islower(68));
	UT_EXPECT_EQ(false, islower(69));
	UT_EXPECT_EQ(false, islower(70));
	UT_EXPECT_EQ(false, islower(71));
	UT_EXPECT_EQ(false, islower(72));
	UT_EXPECT_EQ(false, islower(73));
	UT_EXPECT_EQ(false, islower(74));
	UT_EXPECT_EQ(false, islower(75));
	UT_EXPECT_EQ(false, islower(76));
	UT_EXPECT_EQ(false, islower(77));
	UT_EXPECT_EQ(false, islower(78));
	UT_EXPECT_EQ(false, islower(79));
	UT_EXPECT_EQ(false, islower(80));
	UT_EXPECT_EQ(false, islower(81));
	UT_EXPECT_EQ(false, islower(82));
	UT_EXPECT_EQ(false, islower(83));
	UT_EXPECT_EQ(false, islower(84));
	UT_EXPECT_EQ(false, islower(85));
	UT_EXPECT_EQ(false, islower(86));
	UT_EXPECT_EQ(false, islower(87));
	UT_EXPECT_EQ(false, islower(88));
	UT_EXPECT_EQ(false, islower(89));
	UT_EXPECT_EQ(false, islower(90));
	UT_EXPECT_EQ(false, islower(91));
	UT_EXPECT_EQ(false, islower(92));
	UT_EXPECT_EQ(false, islower(93));
	UT_EXPECT_EQ(false, islower(94));
	UT_EXPECT_EQ(false, islower(95));
	UT_EXPECT_EQ(false, islower(96));
	UT_EXPECT_EQ(false, islower(123));
	UT_EXPECT_EQ(false, islower(124));
	UT_EXPECT_EQ(false, islower(125));
	UT_EXPECT_EQ(false, islower(126));
	UT_EXPECT_EQ(false, islower(127));
}

UT_CREATE_CASE(ctype, islower, negative_inputs, "Negative chars as input")
{
	UT_EXPECT_EQ(false, islower(-159));
}

#endif
