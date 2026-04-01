# =============================================================================
# DISK DRIVERS
# =============================================================================
ROOT_DIR	:= $(shell git rev-parse --show-toplevel)
include $(ROOT_DIR)/build.mk

SRCS		:= ata/disk_ata.c \
			   ata/disk_ata_stat.c \
			   vdl/disk_vdl.c

OBJS		:= $(addsuffix .o, $(SRCS))
DEPS		:= $(OBJS:.o=.d)

-include	$(DEPS)

all: $(OBJS)
