# =============================================================================
# STRING LIB
# =============================================================================
ROOT		:= $(shell git rev-parse --show-toplevel)
include $(ROOT)/config.mk

SRCS		:= memcpy.asm \
			   memcmp.asm \
			   strlen.asm \
			   strcmp.asm

OBJS		:= $(addsuffix .o, $(SRCS))
DEPS		:= $(OBJS:.o=.d)

-include	$(DEPS)

all: $(OBJS)
