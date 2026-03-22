# =============================================================================
# FILE SYSTEMS
# =============================================================================
ROOT		:= $(shell git rev-parse --show-toplevel)
include $(ROOT)/config.mk

SRCS		:= fat/fat_cluster_read.c \
			   fat/fat16/fat16_interface.c \
			   fat/fat16/fat16_probe.c \
			   fat/fat16/fat16_cluster.c \
			   vfs/vfs_mount_table.c \
			   vfs/vfs_partition.c \
			   vfs/vfs_table.c

OBJS		:= $(addsuffix .o, $(SRCS))
DEPS		:= $(OBJS:.o=.d)

-include	$(DEPS)

all: $(OBJS)
