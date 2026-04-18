/*
 * File: ut_run.h
 * Author: Joel Souza
 * Date: 2026-04-17
 * Description: Functions to run Categories, suites or cases by
 * 				solving its global variable names with UT_NAME_* macros
 */

#ifndef UT_RUN_H
# define UT_RUN_H

# include "ut_core.h"
# include "ut_log.h"
# include "ut_name.h"

/*
 * NAME
 * 	ut_run_case, ut_run_suite, ut_run_category -
 * 	Execute unit test hierarchy
 *
 * DESCRIPTION
 * 	These functions execute unit tests while traversing the
 * 	hierarchical structure of the test framework.
 *
 * 	Each function first logs the corresponding component
 * 	description and then executes its children recursively:
 *
 * 	ut_run_case
 * 		Logs and executes a single test case.
 *
 * 	ut_run_suite
 * 		Logs the suite description and executes all test cases
 * 		contained within the suite.
 *
 * 	ut_run_category
 * 		Logs the category description and executes all suites
 * 		contained within the category.
 * */
static inline
void	ut_run_case(const t_case *_case)
{
	UT_LOG_DESCRIPTION(CASE, _case->description);
	_case->test();
}

static inline
void	ut_run_suite(const t_suite *suite)
{
	const uint32_t	entries = suite->entries;
	uint32_t		i;

	UT_LOG_DESCRIPTION(SUITE, suite->description);
	for (i = 0; i < entries; i++)
		ut_run_case(suite->cases[i]);
	vga_text_print("\n", 0);
}

static inline
void	ut_run_category(const t_category *category)
{
	const uint32_t	entries = category->entries;
	uint32_t		i;

	UT_LOG_DESCRIPTION(CATEGORY, category->description);
	for (i = 0; i < entries; i++)
		ut_run_suite(category->suites[i]);
	vga_text_print("\n\n", 0);
}

/*
 * NAME
 * 	UT_RUN_CATEGORY, UT_RUN_SUITE, UT_RUN_CASE -
 * 	Unit test execution dispatch macros
 *
 * DESCRIPTION
 * 	These macros translate test identifiers into their corresponding
 * 	generated symbol names and dispatch execution to the appropriate
 * 	runtime function.
 *
 * 	They rely on the UT_NAME_* macros to resolve category, suite,
 * 	and case names into standardized internal symbols, and then
 * 	invoke the corresponding run functions.
 *
 * 	UT_RUN_CATEGORY(categ)
 * 		Executes all suites belonging to the specified category.
 *
 * 	UT_RUN_SUITE(categ, suite)
 * 		Executes all test cases belonging to the specified suite.
 *
 * 	UT_RUN_CASE(categ, suite, case)
 * 		Executes a single test case.
 * */
# define UT_RUN_CATEGORY(categ) \
ut_run_category(&UT_NAME_CATEGORY(categ))

# define UT_RUN_SUITE(categ, suite) \
ut_run_suite(&UT_NAME_SUITE(categ, suite))

# define UT_RUN_CASE(categ, suite, case) \
ut_run_case(&UT_NAME_CASE(categ, suite, case))

#endif
