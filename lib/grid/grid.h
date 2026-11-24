/*
 * File: grid.h
 * Author: Joel Souza
 * Date: 2026-11-23
 * Description: Library providing data structures and utility functions for 2D grid management
 */

#ifndef GRID_H
# define GRID_H

# include <stdint.h>

typedef struct s_grid_position
{
	uint32_t		column;
	uint32_t		line;
	uint32_t		cell;
}	t_grid_position;

typedef struct s_grid_dimensions
{
	uint32_t		ncolumns;
	uint32_t		nlines;
	uint32_t		ncells;
}	t_grid_dimensions;

typedef struct s_grid_geometry
{
	uint32_t		ncolumns;
	uint32_t		nlines;
	uint32_t		ncells;
	t_grid_position	last_line;
}	t_grid_geometry;

#endif
