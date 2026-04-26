/*
 * File: flags.h
 * Author: Joel Souza
 * Date: 2026-04-26
 * Description: Interface to interact with x86 cpu register flags
 *				in C language
 */

#ifndef CPU_FLAGS_H
# define CPU_FLAGS_H

# include <stdint.h>

# define CPU_FLAGS_DF	0x400

static inline
uint32_t	cpu_flags_get(void)
{
	uint32_t	eflags;

	__asm__(
		"pushfl"	"\n\t"
		"pop	%0"	"\n"
		: "=r"(eflags)
	);
	return (eflags);
}

#endif
