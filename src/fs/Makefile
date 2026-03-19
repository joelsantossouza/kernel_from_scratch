# =============================================================================
# FILE SYSTEMS
# =============================================================================
ROOT	:= $(shell git rev-parse --show-toplevel)
include $(ROOT)/config.mk

OBJS	:= fat/fat16/fat16_interface.c.o \
		   fat/fat16/fat16_probe.c.o \
		   vfs/vfs_mount_table.c.o \
		   vfs/vfs_partition.c.o \
		   vfs/vfs_table.c.o

all: $(OBJS)
