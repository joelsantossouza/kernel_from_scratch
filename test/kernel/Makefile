# =============================================================================
# TEST KERNEL
# =============================================================================
ROOT_DIR			:= $(shell git rev-parse --show-toplevel)
include 			$(ROOT_DIR)/build.mk

TEST_KERNEL_SRCS	:= test_kernel.asm \
					   test_kernel.c \
					   $(IO_DIR)/io.asm \
					   $(VIDEO_DIR)/text/video_text_history.c \
					   $(VIDEO_DIR)/text/video_text_vga.c \
					   $(STRING_DIR)/memcpy.asm \
					   $(STRING_DIR)/memset.asm

TEST_KERNEL_OBJS	:= $(addsuffix .o, $(TEST_KERNEL_SRCS))

BINS				:= $(TEST_KERNEL_NAME).bin
ELFS				:= $(TEST_KERNEL_NAME).elf
OBJS				:= $(TEST_KERNEL_OBJS)
DEPS				:= $(OBJS:.o=.d)
LDFLAGS				+= -T $(TEST_KERNEL_NAME).ld -defsym=CONFIG_KERNEL_ADDR=$(CONFIG_KERNEL_ADDR)
INCLUDE				+= -I$(ROOT_DIR)

-include			$(DEPS)

all: $(BINS) $(ELFS)

$(TEST_KERNEL_NAME).elf: $(TEST_KERNEL_OBJS)
