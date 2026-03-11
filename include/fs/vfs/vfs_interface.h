/*
 * File: vfs_interface.h
 * Author: Joel Souza
 * Date: 2026-03-02
 * Description: Polymorphism structures to create compatibility with any File System
 */

#ifndef VFS_INTERFACE_H
# define VFS_INTERFACE_H

typedef union u_vfs_metadata
{
	t_fat16_metadata	fat16;
}	t_vfs_metadata;

typedef struct s_vfs_interface
{

}	t_vfs_interface;

#endif
