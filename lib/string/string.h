/*
 * File: string.h
 * Author: Joel Souza
 * Date: 2026-03-09
 * Description: String and memory manipulation function declarations
 */

#ifndef STRING_H
# define STRING_H

# include <stdint.h>

void	*memcpy(void *dest, const void *src, uint32_t size);
void	*mempcpy(void *dest, const void *src, uint32_t size);

int		strnlen_strict(const char *str, uint32_t n, uint32_t *len);

#endif
