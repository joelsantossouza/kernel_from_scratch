# =============================================================================
# DISK DRIVERS
# =============================================================================
ROOT	:= $(shell git rev-parse --show-toplevel)
include $(ROOT)/config.mk

OBJS	:= ata/disk_ata.c.o \
		   ata/disk_ata_stat.c.o \
		   vdl/disk_vdl.c.o

all: $(OBJS)
