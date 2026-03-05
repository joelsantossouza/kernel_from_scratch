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
HEADERS			:= $(addprefix $(INCLUDES_DIR)/, \
					boot/config.inc \
					cpu/cr0.inc \
					cpu/gdt.inc \
					drivers/disk/ata/ata.h \
					drivers/disk/vdl/config.inc \
					drivers/disk/vdl/vdl.h \
					drivers/io/io.h \
					fs/fat16/fat16.h \
					fs/vfs/config.h \
					fs/vfs/vfs.h \
					kernel/config.h \
)
