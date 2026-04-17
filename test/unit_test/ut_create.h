/*
 * File: ut_create.h
 * Author: Joel Souza
 * Date: 2026-04-17
 * Description: Creation of Categories, suites and cases of tests
 */

#ifndef UT_CREATE_H
# define UT_CREATE_H

# include "ut_core.h"
# include "ut_name.h"
# include "ut_register.h"

# define UT_CREATE_CATEGORY(categ, descr) \
static t_category	UT_NAME_CATEGORY(categ) = { \
	.suites = {0}, \
	.entries = 0, \
	.description = descr, \
};

# define UT_CREATE_SUITE(categ, suite, descr) \
static t_suite	UT_NAME_SUITE(categ, suite) = { \
	.cases			= {0}, \
	.entries		= 0, \
	.description	= descr, \
}; \
UT_REGISTER_SUITE(categ, suite)

# define UT_CREATE_CASE(categ, suite, case, descr) \
static inline \
void	UT_NAME_CASE_TEST(categ, suite, case)(void); \
static const t_case		UT_NAME_CASE(categ, suite, case) = { \
	.test			= UT_NAME_CASE_TEST(categ, suite, case), \
	.description	= descr, \
}; \
UT_REGISTER_CASE(categ, suite, case) \
static inline \
void	UT_NAME_CASE_TEST(categ, suite, case)(void)

#endif
