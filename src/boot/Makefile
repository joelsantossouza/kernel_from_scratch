# =============================================================================
# BOOT
# =============================================================================
ROOT	:= $(shell git rev-parse --show-toplevel)

include $(ROOT)/config.mk

BINS	:= mbr.bin boot_stage2.bin
ELFS	:= mbr.elf boot_stage2.elf

.PHONY: all fclean re

all: $(BINS) $(ELFS)

%.bin: %.asm $(HEADERS)
	$(AS) $(AFLAGS_BIN) $(INCLUDES) $< -o $@

%.elf: %.asm $(HEADERS)
	$(AS) $(AFLAGS_ELF) $(INCLUDES) $< -o $@

fclean:
	rm -f $(BINS)
	rm -f $(ELFS)

re: fclean all
