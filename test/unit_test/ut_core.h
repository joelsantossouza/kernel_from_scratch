/*
 * File: ut_core.h
 * Author: Joel Souza
 * Date: 2026-04-17
 * Description: Core configurations and properties of unit tester
 */

#ifndef UT_CORE_H
# define UT_CORE_H

# include <stdint.h>
# include "drivers/video/text/video_text.h"
# include "ut_utils.h"

# define UT_LOCATION			__FILE__ ":" UT_STRINGIFY2(__LINE__) ": "

// Colors
# define UT_COLOR_ERROR			VGA_TEXT_DARK_RED
# define UT_COLOR_SUCCESS		VGA_TEXT_LIGHT_GREEN
# define UT_COLOR_FAILURE		VGA_TEXT_LIGHT_RED
# define UT_COLOR_CASE			VGA_TEXT_LIGHT_CYAN
# define UT_COLOR_SUITE			VGA_TEXT_LIGHT_BLUE
# define UT_COLOR_CATEGORY		VGA_TEXT_DARK_BLUE
# define UT_COLOR_LOCATION		VGA_TEXT_LIGHT_GRAY
# define UT_COLOR_CALL			VGA_TEXT_BROWN

// Message Prefixes
# define UT_MSGPFX_ERROR		"[ERROR] "
# define UT_MSGPFX_SUCCESS		"[OK] "
# define UT_MSGPFX_FAILURE		"[KO] "
# define UT_MSGPFX_CASE			"[CASE] "
# define UT_MSGPFX_SUITE		"[SUITE] "
# define UT_MSGPFX_CATEGORY		"[CATEGORY] "
# define UT_MSGPFX_CALL			"[CALLING] "

// Messages
# define UT_MSG_CATEGORY_FULL	"Category capacity is full"
# define UT_MSG_SUITE_FULL		"Suite capacity is full"

typedef void (*t_test_fn)(void);

typedef struct s_case
{
	t_test_fn	test;
	const char	*description;
}	t_case;

# define UT_CASES_PER_SUITE		10
typedef struct s_suite
{
	const t_case	*cases[UT_CASES_PER_SUITE];
	uint32_t		entries;
	const char		*description;
}	t_suite;

# define UT_SUITES_PER_CATEGORY	10
typedef struct s_category
{
	const t_suite	*suites[UT_SUITES_PER_CATEGORY];
	uint32_t		entries;
	const char		*description;
}	t_category;

extern	void (*__start_init_array[])(void);
extern	void (*__stop_init_array[])(void);

/*
 * NAME
 * 	ut_init - Initialize unit test framework
 *
 * DESCRIPTION
 * 	Initializes the unit test system by executing all startup
 * 	functions stored in the `.init_array` ELF section.
 *
 * 	These functions are typically inserted via compiler attributes
 * 	(e.g., __attribute__((constructor))) and are used to perform
 * 	automatic registration of test categories, suites, and cases.
 *
 * 	The function iterates over the range defined by the linker
 * 	symbols `__start_init_array` and `__stop_init_array`, invoking
 * 	each function pointer in sequence.
 *
 * 	This function must be called at the beginning of the program
 * 	to ensure that all test components are properly registered
 * 	before execution.
 * */
static
void	ut_init(void)
{
	void (**fn_startup)(void);

	fn_startup = __start_init_array;
	while (fn_startup < __stop_init_array)
		(*fn_startup++)();
}

#endif
