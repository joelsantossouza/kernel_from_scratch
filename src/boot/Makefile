# =============================================================================
# BOOT
# =============================================================================
ROOT		:= $(shell git rev-parse --show-toplevel)

include $(ROOT)/config.mk

BOOT_BIN	:= boot_stage2.bin
BOOT_OBJS	:= boot_stage2.asm.o

BINS		:= mbr.bin boot_stage2.bin
ELFS		:= mbr.elf boot_stage2.elf

LINKER		:= linker.ld

.PHONY: all fclean re

all: $(BINS) $(ELFS)

$(BOOT_BIN): $(BOOT_OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -T $(LINKER) $^ -o $@

%.bin: %.asm $(HEADERS)
	$(AS) $(AFLAGS_BIN) $(INCLUDES) $< -o $@

%.elf: %.asm $(HEADERS)
	$(AS) $(AFLAGS_ELF) $(INCLUDES) $< -o $@

%.asm.o: %.asm $(HEADERS)
	$(AS) $(AFLAGS_ELF) $(INCLUDES) $< -o $@

%.c.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

fclean:
	rm -f $(BINS)
	rm -f $(ELFS)
	rm -f $(BOOT_OBJS)

re: fclean all
