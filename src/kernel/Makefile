# =============================================================================
# KERNEL
# =============================================================================
ROOT		:= $(shell git rev-parse --show-toplevel)
include $(ROOT)/config.mk

KERNEL_NAME	:= kernel
KERNEL_SRCS	:= kernel.asm
KERNEL_OBJS	:= $(addsuffix .o, $(KERNEL_SRCS))

BINS		:= $(KERNEL_NAME).bin
ELFS		:= $(KERNEL_NAME).elf
OBJS		:= $(KERNEL_OBJS)
DEPS		:= $(OBJS:.o=.d)

-include	$(DEPS)

all: $(BINS) $(ELFS)

$(KERNEL_NAME).bin $(KERNEL_NAME).elf: $(KERNEL_OBJS)
