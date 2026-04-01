# =============================================================================
# FILE SYSTEMS
# =============================================================================
ROOT_DIR	:= $(shell git rev-parse --show-toplevel)
include $(ROOT_DIR)/build.mk

SRCS		:= fat/fat_cluster_read.c \
			   fat/fat16/fat16_interface.c \
			   fat/fat16/fat16_probe.c \
			   vfs/vfs_mount_table.c \
			   vfs/vfs_partition.c \
			   vfs/vfs_table.c

OBJS		:= $(addsuffix .o, $(SRCS))
DEPS		:= $(OBJS:.o=.d)

-include	$(DEPS)

all: $(OBJS)
