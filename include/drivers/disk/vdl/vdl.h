/*
 * File: vdl.h
 * Author: Joel Souza
 * Date: 2026-03-03
 * Description: Virtual Disk Layer (VDL) structures and abstractions
 */

#ifndef VDL_H
# define VDL_H

# include <stdint.h>
# include <sys/types.h>

typedef struct s_vdl_disk	t_vdl_disk;

typedef struct s_vdl
{
}	t_vdl;

typedef struct s_vdl_disk
{
	uint8_t		no;
	const t_vdl	*vdl;
}	t_vdl_disk;

#endif
