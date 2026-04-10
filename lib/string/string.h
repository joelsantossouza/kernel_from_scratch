/*
 * File: string.h
 * Author: Joel Souza
 * Date: 2026-03-09
 * Description: String and memory manipulation function declarations
 */

#ifndef STRING_H
# define STRING_H

# include <stdint.h>

void	*memcpy(void *dest, const void *src, uint32_t n);
void	*mempcpy(void *dest, const void *src, uint32_t n);
void	*memmove(void *dest, const void *src, uint32_t n);
void	*memicpy8(void *dest, const void *src, uint32_t n, uint8_t attr);
void	*memset(void *dest, int c, uint32_t n);
int		memcmp(const void *s1, const void *s2, uint32_t n);

int		strnlen_strict(const char *str, uint32_t n, uint32_t *len);
int		strcmp(const char *s1, const char *s2);

#endif
