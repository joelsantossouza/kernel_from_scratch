# =============================================================================
# DISK DRIVERS
# =============================================================================
ROOT		:= $(shell git rev-parse --show-toplevel)
include $(ROOT)/config.mk

SRCS		:= ata/disk_ata.c \
			   ata/disk_ata_stat.c \
			   vdl/disk_vdl.c

OBJS		:= $(addsuffix .o, $(SRCS))
DEPS		:= $(OBJS:.o=.d)

-include	$(DEPS)

all: $(OBJS)
