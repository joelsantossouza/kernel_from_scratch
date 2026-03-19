# =============================================================================
# MAKEFILE CONFIG
# =============================================================================
ROOT			:= $(shell git rev-parse --show-toplevel)

CC				:= gcc
AS				:= nasm
LD				:= ld

CFLAGS			:= -m32 -fno-pic -fno-pie -Wall -Wextra -Werror -g -ffreestanding -nostdlib -fno-builtin
AFLAGS			:= -w+all -w+error -f elf32 -g -F dwarf
LDFLAGS			:= -m elf_i386 -T linker.ld -nostdlib --no-undefined

# Directories
# ---------------------------------------
INCLUDES_DIR	:= $(ROOT)/include
LIBS_DIR		:= $(ROOT)/lib
SRCS_DIR		:= $(ROOT)/src

DRIVERS_DIR		:= $(SRCS_DIR)/drivers
IO_DIR			:= $(DRIVERS_DIR)/io
DISK_DIR		:= $(DRIVERS_DIR)/disk

FS_DIR			:= $(SRCS_DIR)/fs

STRING_DIR		:= $(LIBS_DIR)/string

INCLUDES		:= -I$(INCLUDES_DIR) -I$(LIBS_DIR)

.DEFAULT_GOAL	= all

# =============================================================================
# Geral Build
# =============================================================================
.PHONY: all clean fclean re
.SECONDARY:

# Compile Objects
# ---------------------------------------
%.c.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@

# Assembly Objects
# ---------------------------------------
%.asm.o: %.asm
	$(AS) $(AFLAGS) $(INCLUDES) -MD $(@:.o=.d) $< -o $@

# Link Binary
# ---------------------------------------
%.bin:
	$(LD) $(LDFLAGS) --oformat binary $^ -o $@

# Link Elf
# ---------------------------------------
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
