# =============================================================================
# STRING LIB
# =============================================================================
ROOT		:= $(shell git rev-parse --show-toplevel)
include $(ROOT)/config.mk

OBJS		:= memcpy.asm.o \
			   memcmp.asm.o \
			   strlen.asm.o \
			   strcmp.asm.o
DEPS		:= $(OBJS:.o=.d)

-include	$(DEPS)

all: $(OBJS)
