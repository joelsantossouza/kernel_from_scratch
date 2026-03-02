# =============================================================================
# MAKEFILE CONFIG
# =============================================================================
CC				:= gcc
AS				:= nasm

CFLAGS			:= -m32 -Wall -Wextra -Werror -g -ffreestanding -nostdlib -fno-builtin
AFLAGS			:= -w+all -w+error -g

INCLUDES_DIR	:= $(ROOT)/include

INCLUDES		:= -I$(INCLUDES_DIR)
HEADERS			:= $(addprefix $(INCLUDES_DIR)/, \
					boot/config.inc \
					cpu/cr0.inc \
					cpu/gdt.inc \
					drivers/disk/config.inc \
)
