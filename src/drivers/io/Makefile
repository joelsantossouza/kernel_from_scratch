# =============================================================================
# IO DRIVERS
# =============================================================================
ROOT	:= $(shell git rev-parse --show-toplevel)

include $(ROOT)/config.mk

OBJS	:= io.asm.o

.PHONY: all fclean

all: $(OBJS)

%.asm.o: %.asm $(HEADERS)
	$(AS) $(AFLAGS_ELF) $(INCLUDES) $< -o $@

fclean:
	rm -f $(OBJS)

re: fclean all
