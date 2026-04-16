/*
 * File: unit_test.h
 * Author: Joel Souza
 * Date: 2026-04-14
 * Description: Assertion and expect macros utilities to track
 * 				successes and failures with clear outputs.
 */

#ifndef UNIT_TEST_H
# define UNIT_TEST_H

# include <stdbool.h>
# include "drivers/video/text/video_text.h"

// Ensure true and false are bool type
# undef true
# undef false
# define true						((bool)1)
# define false						((bool)0)

// Deeper Stringification
# define UT_STRINGIFY2(x)			#x
# define UT_STRINGIFY(x)			UT_STRINGIFY2(x)

// Concatenations
# define UT_CAT(a, b)				a##b
# define UT_CAT2(a, b)				UT_CAT(a, b)
# define UT_CAT3(a, b, c)			UT_CAT2(UT_CAT2(a, b), c)
# define UT_CAT4(a, b, c, d)		UT_CAT2(UT_CAT3(a, b, c), d)
# define UT_CAT5(a, b, c, d, e)		UT_CAT2(UT_CAT4(a, b, c, d), e)
# define UT_CAT6(a, b, c, d, e, f)	UT_CAT2(UT_CAT5(a, b, c, d, e), f)

// Colors
# define UT_COLOR_SUCCESS			VGA_TEXT_LIGHT_GREEN
# define UT_COLOR_FAILURE			VGA_TEXT_DARK_RED
# define UT_COLOR_LOCATION			VGA_TEXT_WHITE
# define UT_COLOR_ERROR				VGA_TEXT_DARK_RED
# define UT_COLOR_CASE				VGA_TEXT_LIGHT_CYAN
# define UT_COLOR_SUITE				VGA_TEXT_LIGHT_BLUE
# define UT_COLOR_CATEGORY			VGA_TEXT_DARK_BLUE

// Message Prefixes
# define UT_MSGPFX_SUCCESS			"[OK] "
# define UT_MSGPFX_FAILURE			"[KO] "
# define UT_MSGPFX_LOCATION			""
# define UT_MSGPFX_ERROR			"[ERROR] "
# define UT_MSGPFX_CASE				"[CASE] "
# define UT_MSGPFX_SUITE			"[SUITE] "
# define UT_MSGPFX_CATEGORY			"[CATEGORY] "

// Messages
# define UT_MSG_LOCATION			__FILE__ ":" UT_STRINGIFY(__LINE__) ": "

// Log
# define UT_LOG(status, msg) \
do \
{ \
	vga_text_print(UT_MSGPFX_LOCATION UT_MSG_LOCATION, UT_COLOR_LOCATION); \
	vga_text_print(UT_MSGPFX_##status msg "\n", UT_COLOR_##status); \
} \
while (false)

// Assertion
# define UT_EXPECT_EQ(expect, actual) \
do \
{ \
	bool is_equal = _Generic((expect), \
						  bool: (expect) == !!(actual) \
					); \
	if (is_equal) \
		UT_LOG(SUCCESS, #actual " == " #expect); \
	else \
		UT_LOG(FAILURE, #actual " == " #expect); \
} \
while (false)

// Test Error Handler
static
void	ut_error(const char *msg)
{
	vga_text_print(UT_MSGPFX_ERROR, UT_COLOR_ERROR);
	vga_text_print(msg, UT_COLOR_ERROR);
	vga_text_print("\n", UT_COLOR_ERROR);
	while (true)
		;
}

// Test Names Generations
# define UT_NAME_CATEGORY(categ) \
UT_CAT2(ut_categ_, categ)

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

// Test Buffer Sizes
# define UT_CASES_PER_SUITE		100
# define UT_SUITES_PER_CATEGORY	100

// Test Structures
typedef void (*t_test_fn)(void);

typedef struct s_case
{
	t_test_fn	test;
	const char	*description;
}	t_case;

typedef struct s_suite
{
	const t_case	*cases[UT_CASES_PER_SUITE];
	uint32_t		entries;
	const char		*description;
}	t_suite;

typedef struct s_category
{
	const t_suite	*suites[UT_SUITES_PER_CATEGORY];
	uint32_t		entries;
	const char		*description;
}	t_category;

// Test Init
extern	void (*__start_init_array[])(void);
extern	void (*__stop_init_array[])(void);

static
void	ut_startup(void)
{
	void (**fn_startup)(void);

	fn_startup = __start_init_array;
	while (fn_startup < __stop_init_array)
		(*fn_startup++)();
}

// Test runs
static inline
void	ut_run_case(const t_case *_case)
{
	vga_text_print(UT_MSGPFX_CASE, UT_COLOR_CASE);
	vga_text_print(_case->description, UT_COLOR_CASE);
	vga_text_print("\n", UT_COLOR_CASE);
	_case->test();
}

# define UT_RUN_CASE(categ, suite, case) \
ut_run_case(&UT_NAME_CASE(categ, suite, case))

static inline
void	ut_run_suite(const t_suite *suite)
{
	const uint32_t	entries = suite->entries;
	uint32_t		i;

	vga_text_print(UT_MSGPFX_SUITE, UT_COLOR_SUITE);
	vga_text_print(suite->description, UT_COLOR_SUITE);
	vga_text_print("\n", UT_COLOR_SUITE);
	for (i = 0; i < entries; i++)
		ut_run_case(suite->cases[i]);
}

# define UT_RUN_SUITE(categ, suite) \
ut_run_suite(&UT_NAME_SUITE(categ, suite))

static inline
void	ut_run_category(const t_category *category)
{
	const uint32_t	entries = category->entries;
	uint32_t		i;

	vga_text_print(UT_MSGPFX_CATEGORY, UT_COLOR_CATEGORY);
	vga_text_print(category->description, UT_COLOR_CATEGORY);
	vga_text_print("\n", UT_COLOR_CATEGORY);
	for (i = 0; i < entries; i++)
		ut_run_suite(category->suites[i]);
}

# define UT_RUN_CATEGORY(categ) \
ut_run_category(&UT_NAME_CATEGORY(categ))

// Test Register
static inline
void	ut_register_suite(t_category *category, const t_suite *suite)
{
	if (category->entries >= UT_SUITES_PER_CATEGORY)
		ut_error(UT_MSG_LOCATION "Category capacity exceeded");
	category->suites[category->entries++] = suite;
}

# define UT_REGISTER_SUITE(categ, suite) \
__attribute__((constructor)) \
static \
void	UT_NAME_SUITE_REGISTER(categ, suite)(void) \
{ \
	ut_register_suite(&UT_NAME_CATEGORY(categ), &UT_NAME_SUITE(categ, suite)); \
}

static inline
void	ut_register_case(t_suite *suite, const t_case *_case)
{
	if (suite->entries >= UT_CASES_PER_SUITE)
		ut_error(UT_MSG_LOCATION "Suite capacity exceeded");
	suite->cases[suite->entries++] = _case;
}

# define UT_REGISTER_CASE(categ, suite, case) \
__attribute__((constructor)) \
static \
void	UT_NAME_CASE_REGISTER(categ, suite, case)(void) \
{ \
	ut_register_case(&UT_NAME_SUITE(categ, suite), &UT_NAME_CASE(categ, suite, case)); \
}

// Test Create
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
