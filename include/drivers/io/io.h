/*
 * File: io.h
 * Author: Joel Souza
 * Date: 2026-03-02
 * Description: I/O ports assembly drivers prototypes to usage in C
 */

#ifndef IO_H
# define IO_H

# include <stdint.h>

uint8_t		io_inb(uint16_t port);
uint16_t	io_inw(uint16_t port);
uint32_t	io_ind(uint16_t port);

void		io_outb(uint16_t port, uint8_t value);
void		io_outw(uint16_t port, uint16_t value);
void		io_outd(uint16_t port, uint32_t value);

#endif
