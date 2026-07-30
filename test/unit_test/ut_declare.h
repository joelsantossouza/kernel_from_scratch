/*
 * File: ut_declare.h
 * Author: Joel Souza
 * Date: 2026-07-30
 * Description: Declaration of categories, suites and cases of tests
 */

#ifndef UT_DECLARE_H
# define UT_DECLARE_H

# include "ut_core.h"
# include "ut_name.h"

/*
 * TODO:
 * */
# define UT_DECLARE_CATEGORY(categ) \
t_category	UT_NAME_CATEGORY(categ)

# define UT_DECLARE_SUITE(categ, suite) \
t_suite	UT_NAME_SUITE(categ, suite)

# define UT_DECLARE_CASE(categ, suite, case) \
const t_case	UT_NAME_CASE(categ, suite, case)

#endif
