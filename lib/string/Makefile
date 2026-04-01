# =============================================================================
# STRING LIB
# =============================================================================
ROOT_DIR	:= $(shell git rev-parse --show-toplevel)
include $(ROOT_DIR)/build.mk

SRCS		:= memcpy.asm \
			   memcmp.asm \
			   strlen.asm \
			   strcmp.asm

OBJS		:= $(addsuffix .o, $(SRCS))
DEPS		:= $(OBJS:.o=.d)

-include	$(DEPS)

all: $(OBJS)
