# =============================================================================
# IO DRIVERS
# =============================================================================
ROOT_DIR	:= $(shell git rev-parse --show-toplevel)
include $(ROOT_DIR)/build.mk

SRCS		:= io.asm
OBJS		:= $(addsuffix .o, $(SRCS))
DEPS		:= $(OBJS:.o=.d)

-include	$(DEPS)

all: $(OBJS)
