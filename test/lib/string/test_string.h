/*
 * File: test_string.h
 * Author: Joel Souza
 * Date: 2026-04-20
 * Description: Test string library functions
 */

#ifndef TEST_STRING_H
# define TEST_STRING_H

# include "test/unit_test/unit_test.h"

# undef SRC_SIZE
# define SRC_SIZE			4096

# undef SRC_PATTERN
# define SRC_PATTERN		"Very Aligned Memory!"

# undef SRC_PATTERN_SIZE
# define SRC_PATTERN_SIZE	20

# undef SRC_ATTR
# define SRC_ATTR			'-'

# undef SRC_UNIFORM_BYTE
# define SRC_UNIFORM_BYTE	0x42

# undef SRC_UNIFORM_WORD
# define SRC_UNIFORM_WORD	0xC0FE

# define ALIGN_32			4

extern UT_DECLARE_CATEGORY(string);

#endif
