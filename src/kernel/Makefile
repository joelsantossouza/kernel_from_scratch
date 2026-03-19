# =============================================================================
# KERNEL
# =============================================================================
ROOT		:= $(shell git rev-parse --show-toplevel)
include $(ROOT)/config.mk

KERNEL_BIN	:= kernel.bin
KERNEL_ELF	:= kernel.elf
KERNEL_OBJS	:= kernel.asm.o

BINS		:= $(KERNEL_BIN)
ELFS		:= $(KERNEL_ELF)
OBJS		:= $(KERNEL_OBJS)
DEPS		:= $(OBJS:.o=.d)

-include	$(DEPS)

all: $(BINS) $(ELFS)

$(KERNEL_BIN) $(KERNEL_ELF): $(KERNEL_OBJS)
