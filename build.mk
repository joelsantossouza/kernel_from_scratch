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
DEFINES			:=
.DEFAULT_GOAL	= all

# Commands
define register_config
DEFINES			+= -D$(1)=$(strip $($(1)))
endef

# =============================================================================
# Geral Build
# =============================================================================
.PHONY: all clean fclean re
.SECONDARY:

# Compile Objects
%.c.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) $(DEFINES) -MMD -MP -c $< -o $@

# Assembly Objects
%.asm.o: %.asm
	$(AS) $(AFLAGS) $(INCLUDES) $(DEFINES) -MD $(@:.o=.d) $< -o $@

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
