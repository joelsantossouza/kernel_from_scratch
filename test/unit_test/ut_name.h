/*
 * File: ut_name.h
 * Author: Joel Souza
 * Date: 2026-04-17
 * Description: Unit test names generation
 */

#ifndef UT_NAME_H
# define UT_NAME_H

# include "ut_utils.h"

/*
 * NAME
 * 	UT_NAME_CATEGORY, UT_NAME_SUITE, UT_NAME_SUITE_REGISTER,
 * 	UT_NAME_CASE, UT_NAME_CASE_TEST, UT_NAME_CASE_REGISTER
 * 	- Unit test symbol name generation macros
 *
 * DESCRIPTION
 * 	These macros generate standardized symbol names for unit test
 * 	components using token concatenation.
 *
 * 	Naming follows a hierarchical pattern:
 *
 * 		ut_category_<categ>_suite_<suite>_case_<case>
 *
 * 	Additional suffixes are appended depending on the symbol type:
 *
 * 		_test       → test function
 * 		_register   → registration function
 *
 * 	This ensures consistent and collision-free naming across
 * 	categories, suites, and test cases.
 * */
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
