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
}

static inline
void	ut_run_category(const t_category *category)
{
	const uint32_t	entries = category->entries;
	uint32_t		i;

	UT_LOG_DESCRIPTION(CATEGORY, category->description);
	for (i = 0; i < entries; i++)
		ut_run_suite(category->suites[i]);
}

# define UT_RUN_CATEGORY(categ) \
ut_run_category(&UT_NAME_CATEGORY(categ))

# define UT_RUN_SUITE(categ, suite) \
ut_run_suite(&UT_NAME_SUITE(categ, suite))

# define UT_RUN_CASE(categ, suite, case) \
ut_run_case(&UT_NAME_CASE(categ, suite, case))

#endif
