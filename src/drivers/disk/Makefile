# =============================================================================
# DISK DRIVERS
# =============================================================================
ROOT		:= $(shell git rev-parse --show-toplevel)

include $(ROOT)/config.mk

all: $(OBJS)

%.c.o: %.c $(HEADERS)
	$(CC)
