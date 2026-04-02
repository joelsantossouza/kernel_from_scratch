# =============================================================================
# BOILERPLATE MAKEFILE
# =============================================================================
# OS component names
MBR_NAME		:= mbr
BOOTLOADER_NAME	:= boot_stage2
KERNEL_NAME		:= kernel
OS_NAME			:= MyOs

# Directories
ROOT_DIR		:= $(shell git rev-parse --show-toplevel)

INCLUDES_DIR	:= $(ROOT_DIR)/include
SRCS_DIR		:= $(ROOT_DIR)/src
LIBS_DIR		:= $(ROOT_DIR)/lib

DRIVERS_DIR		:= $(SRCS_DIR)/drivers
FS_DIR			:= $(SRCS_DIR)/fs

IO_DIR			:= $(DRIVERS_DIR)/io
DISK_DIR		:= $(DRIVERS_DIR)/disk

STRING_DIR		:= $(LIBS_DIR)/string

# Build Configs
CC				:= gcc
AS				:= nasm
LD				:= ld
CFLAGS			:= -m32 -fno-pic -fno-pie -Wall -Wextra -Werror -g -ffreestanding -nostdlib -fno-builtin
AFLAGS			:= -w+all -w+error -f elf32 -g -F dwarf
LDFLAGS			:= -m elf_i386 -nostdlib --no-undefined
INCLUDES		:= -I$(INCLUDES_DIR) -I$(LIBS_DIR)
.DEFAULT_GOAL	= all

# Commands
define assert_options
$(if $(filter $(1),$($(2))),,\
	$(error $(2) must be one of: $(1) | current value = $($(2))))
endef

define assert_range
$(if $(shell \
    val=$$(printf "%d" $($(3))); \
    min=$$(printf "%d" $(1)); \
    max=$$(printf "%d" $(2)); \
    [ $$val -ge $$min ] && [ $$val -le $$max ] && echo OK),,\
    $(error $(3) must be between $(1) and $(2) | current value = $($(3))))
endef

# =============================================================================
# Geral Build
# =============================================================================
.PHONY: all clean fclean re
.SECONDARY:

# Compile Objects
%.c.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@

# Assembly Objects
%.asm.o: %.asm
	$(AS) $(AFLAGS) $(INCLUDES) -MD $(@:.o=.d) $< -o $@

# Link Binary
%.bin:
	$(LD) $(LDFLAGS) --oformat binary $^ -o $@

# Link Elf
%.elf:
	$(LD) $(LDFLAGS) --oformat elf32-i386 $^ -o $@

# =============================================================================
# Clean up
# =============================================================================
clean:
	rm -f $(DEPS)
	rm -f $(OBJS)

fclean: clean
	rm -f $(BINS)
	rm -f $(ELFS)

re: fclean all
