# =============================================================================
# IO DRIVERS
# =============================================================================
ROOT		:= $(shell git rev-parse --show-toplevel)
include $(ROOT)/config.mk

OBJS		:= io.asm.o
DEPS		:= $(OBJS:.o=.d)

-include	$(DEPS)

all: $(OBJS)
