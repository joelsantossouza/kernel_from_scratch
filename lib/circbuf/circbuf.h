/*
 * File: circbuf.h
 * Author: Joel Souza
 * Date: 2026-11-21
 * Description: Circular Buffer management functions
 */

#ifndef CIRCBUF_H
# define CIRCBUF_H

# include <stdint.h>
# include "math/math.h"

static inline
uint32_t	circbuf_offset_advance(uint32_t offset, uint32_t amount, uint32_t bound)
{
	offset += amount;
	if (offset < bound)
		return (offset);
	return (offset % bound);
}

static inline
uint32_t	circbuf_offset_advance_bounded(uint32_t offset_bounded, uint32_t amount_bounded, uint32_t bound)
{
	const uint32_t	offset_next = offset_bounded + amount_bounded;

	if (offset_next < bound)
		return (offset_next);
	return (offset_next - bound);
}

static inline
uint32_t	circbuf_offset_rewind_bounded(uint32_t offset_bounded, uint32_t amount_bounded, uint32_t bound)
{
	if (offset_bounded < amount_bounded)
		return (offset_bounded + bound - amount_bounded);
	return (offset_bounded - amount_bounded);
}

static inline
uint32_t	circbuf_size_increment(uint32_t size, uint32_t amount, uint32_t bound)
{
	return(MIN(size + amount, bound));
}

#endif
