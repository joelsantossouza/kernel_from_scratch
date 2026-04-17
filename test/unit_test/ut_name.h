/*
 * File: ut_name.h
 * Author: Joel Souza
 * Date: 2026-04-17
 * Description: Unit test names generation
 */

#ifndef UT_NAME_H
# define UT_NAME_H

# include "ut_utils.h"

# define UT_NAME_CATEGORY(categ) \
UT_CAT2(ut_category_, categ)

# define UT_NAME_SUITE(categ, suite) \
UT_CAT3(UT_NAME_CATEGORY(categ), _suite_, suite)

# define UT_NAME_SUITE_REGISTER(categ, suite) \
UT_CAT2(UT_NAME_SUITE(categ, suite), _register)

# define UT_NAME_CASE(categ, suite, case) \
UT_CAT3(UT_NAME_SUITE(categ, suite), _case_, case)

# define UT_NAME_CASE_TEST(categ, suite, case) \
UT_CAT2(UT_NAME_CASE(categ, suite, case), _test)

# define UT_NAME_CASE_REGISTER(categ, suite, case) \
UT_CAT2(UT_NAME_CASE(categ, suite, case), _register)

#endif
