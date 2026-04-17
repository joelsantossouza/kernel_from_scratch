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

// Message Prefixes
# define UT_MSGPFX_ERROR		"[ERROR] "
# define UT_MSGPFX_SUCCESS		"[OK] "
# define UT_MSGPFX_FAILURE		"[KO] "
# define UT_MSGPFX_CASE			"[CASE] "
# define UT_MSGPFX_SUITE		"[SUITE] "
# define UT_MSGPFX_CATEGORY		"[CATEGORY] "

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

static
void	ut_init(void)
{
	void (**fn_startup)(void);

	fn_startup = __start_init_array;
	while (fn_startup < __stop_init_array)
		(*fn_startup++)();
}

#endif
