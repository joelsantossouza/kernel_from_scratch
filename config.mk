# =============================================================================
# MAKEFILE CONFIG
# =============================================================================
ROOT			:= $(shell git rev-parse --show-toplevel)

CC				:= gcc
AS				:= nasm

CFLAGS			:= -m32 -fno-pic -fno-pie -Wall -Wextra -Werror -g -ffreestanding -nostdlib -fno-builtin
LDFLAGS			:= -m32 -no-pie -nostdlib -g
AFLAGS			:= -w+all -w+error
AFLAGS_BIN		:= $(AFLAGS) -f bin
AFLAGS_ELF		:= $(AFLAGS) -f elf32 -g -F dwarf

INCLUDES_DIR	:= $(ROOT)/include

INCLUDES		:= -I$(INCLUDES_DIR)
