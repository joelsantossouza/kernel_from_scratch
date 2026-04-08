# =============================================================================
# KERNEL
# =============================================================================
ROOT_DIR	:= $(shell git rev-parse --show-toplevel)
include 	$(ROOT_DIR)/build.mk

KERNEL_SRCS	:= kernel.asm
KERNEL_OBJS	:= $(addsuffix .o, $(KERNEL_SRCS))

BINS		:= $(KERNEL_NAME).bin
ELFS		:= $(KERNEL_NAME).elf
OBJS		:= $(KERNEL_OBJS)
DEPS		:= $(OBJS:.o=.d)

-include	$(DEPS)

all: $(BINS) $(ELFS)

$(KERNEL_NAME).elf: $(KERNEL_OBJS)
