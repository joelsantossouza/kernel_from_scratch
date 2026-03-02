# =============================================================================
# BOOT
# =============================================================================
ROOT	:= $(shell git rev-parse --show-toplevel)

include $(ROOT)/config.mk

BINS	:= mbr.bin stage2.bin

all: $(BINS)

%.bin: %.asm $(HEADERS)
	$(AS) -f bin $(AFLAGS) $(INCLUDES) $< -o $@

fclean:
	rm -f $(BINS)

re: fclean all
