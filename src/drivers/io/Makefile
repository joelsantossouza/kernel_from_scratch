# =============================================================================
# IO DRIVERS
# =============================================================================
ROOT		:= $(shell git rev-parse --show-toplevel)
include $(ROOT)/config.mk

SRCS		:= io.asm
OBJS		:= $(addsuffix .o, $(SRCS))
DEPS		:= $(OBJS:.o=.d)

-include	$(DEPS)

all: $(OBJS)
