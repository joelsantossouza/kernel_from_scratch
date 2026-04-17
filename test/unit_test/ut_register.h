/*
 * File: ut_register.h
 * Author: Joel Souza
 * Date: 2026-04-17
 * Description: Register suites/cases on global structures at program
 *				startup to track its metadata informations
 */

#ifndef UT_REGISTER_H
# define UT_REGISTER_H

# include "ut_core.h"
# include "ut_log.h"

/*
 * NAME
 * 	ut_register_suite,
 * 	ut_register_case - Unit test component registration
 *
 * DESCRIPTION
 * 	These functions register unit test components into their
 * 	respective parent groups.
 *
 * 	ut_register_suite inserts a test suite into a category.
 * 	ut_register_case inserts a test case into a suite.
 *
 * 	On success, the component pointer is stored and the number
 * 	of entries is incremented.
 *
 * 	If the target group has reached its maximum capacity, an
 *	ERROR log message is emitted and execution is halted.
 * */
static inline
void	ut_register_suite(t_category *category, const t_suite *suite)
{
	if (category->entries >= UT_SUITES_PER_CATEGORY)
		UT_LOG_FATAL(ERROR, UT_MSG_CATEGORY_FULL);
	category->suites[category->entries++] = suite;
}

static inline
void	ut_register_case(t_suite *suite, const t_case *_case)
{
	if (suite->entries >= UT_CASES_PER_SUITE)
		UT_LOG_FATAL(ERROR, UT_MSG_SUITE_FULL);
	suite->cases[suite->entries++] = _case;
}

/*
 * NAME
 * 	UT_REGISTER_SUITE, UT_REGISTER_CASE -
 * 	Unit test startup registration macros
 *
 * DESCRIPTION
 * 	These macros define static constructor functions that are
 * 	executed at program startup. Each constructor performs automatic
 * 	registration of a unit test component into its parent group.
 *
 * 	UT_REGISTER_SUITE registers a test suite into its category.
 * 	UT_REGISTER_CASE registers a test case into its suite.
 *
 * 	The constructors are placed in the `.init_array` section via
 * 	__attribute__((constructor)), allowing the unit test hierarchy
 * 	to be fully initialized before execution begins throught ut_init()
 * 	invocation.
 * */
# define UT_REGISTER_SUITE(categ, suite) \
__attribute__((constructor)) \
static \
void	UT_NAME_SUITE_REGISTER(categ, suite)(void) \
{ \
	ut_register_suite(&UT_NAME_CATEGORY(categ), &UT_NAME_SUITE(categ, suite)); \
}

# define UT_REGISTER_CASE(categ, suite, case) \
__attribute__((constructor)) \
static \
void	UT_NAME_CASE_REGISTER(categ, suite, case)(void) \
{ \
	ut_register_case(&UT_NAME_SUITE(categ, suite), &UT_NAME_CASE(categ, suite, case)); \
}

#endif
