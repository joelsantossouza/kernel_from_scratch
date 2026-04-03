# =============================================================================
# BOILERPLATE MAKEFILE
# =============================================================================
.DEFAULT_GOAL	= all

# Directories
ROOT_DIR		:= $(shell git rev-parse --show-toplevel)

SRC_DIR			:= $(ROOT_DIR)/src
LIB_DIR			:= $(ROOT_DIR)/lib
INC_DIR			:= $(ROOT_DIR)/include

BOOT_DIR		:= $(SRC_DIR)/boot
KERNEL_DIR		:= $(SRC_DIR)/kernel
DRIVERS_DIR		:= $(SRC_DIR)/drivers
FS_DIR			:= $(SRC_DIR)/fs

IO_DIR			:= $(DRIVERS_DIR)/io
DISK_DIR		:= $(DRIVERS_DIR)/disk

STRING_DIR		:= $(LIB_DIR)/string

# OS Components
OS_NAME			:= HavenOS
OS				:= ./$(OS_NAME)

MBR_NAME		:= mbr
MBR				:= $(BOOT_DIR)/$(MBR_NAME)

BOOTLOADER_NAME	:= boot_stage2
BOOTLOADER		:= $(BOOT_DIR)/$(BOOTLOADER_NAME)

KERNEL_NAME		:= kernel
KERNEL			:= $(KERNEL_DIR)/$(KERNEL_NAME)

PART0_NAME		:= part0
PART0			:= ./$(PART0_NAME)

PART1_NAME		:= part1
PART1			:= ./$(PART1_NAME)

PART2_NAME		:= part2
PART2			:= ./$(PART2_NAME)

PART3_NAME		:= part3
PART3			:= ./$(PART3_NAME)

# Tools
CC				:= gcc
AS				:= nasm
LD				:= ld
DD				:= dd

# Flags
CFLAGS			:= -m32 -fno-pic -fno-pie -Wall -Wextra -Werror -g -ffreestanding -nostdlib -fno-builtin
AFLAGS			:= -w+all -w+error -f elf32 -g -F dwarf
LDFLAGS			:= -m elf_i386 -nostdlib --no-undefined
DDFLAGS			:=
INCLUDE			:= -I$(INC_DIR) -I$(LIB_DIR)
DEFINE			:=

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

define assert_range_span
$(if $(shell \
    start=$$(printf "%d" $($(3))); \
    count=$$(printf "%d" $(4)); \
    size=$$(printf "%d" $(5)); \
    min=$$(printf "%d" $(1)); \
    max=$$(printf "%d" $(2)); \
    end=$$((start + count * size)); \
    [ $$start -ge $$min ] && [ $$end -le $$max ] && echo OK),,\
    $(error $(3) span [$($(3)), \
$(shell printf "0x%x" $$(( $$(printf "%d" $($(3))) + $$(printf "%d" $(4)) * $$(printf "%d" $(5)) )))] \
must fit between $(1) and $(2)))
endef

define assert_align
$(if $(filter-out 0,$(shell printf "%d" $$(( $($(1)) % $(2) )))), \
    $(error $(1) is not aligned to $(2) | current value = $($(1))),)
endef

# OS Values
include						$(ROOT_DIR)/.config
CONFIG_DISK_PART0_SECTORS	:= $(shell printf "%d" $$(( \
							   $(CONFIG_DISK_PART0_END) - $(CONFIG_DISK_PART0_START) \
)))

# =============================================================================
# Build
# =============================================================================
.PHONY: all clean fclean re
.SECONDARY:

# C Objects
%.c.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE) -MMD -MP -c $< -o $@

# Assembly Objects
%.asm.o: %.asm
	$(AS) $(AFLAGS) $(INCLUDE) -MD $(@:.o=.d) $< -o $@

# Binary
%.bin:
	$(LD) $(LDFLAGS) --oformat binary $^ -o $@

# Elf
%.elf:
	$(LD) $(LDFLAGS) --oformat elf32-i386 $^ -o $@

# Partitions
$(PART0).img:
	$(DD) $(DDFLAGS) if=/dev/zero of=$@ count=$(CONFIG_DISK_PART0_SECTORS)

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
