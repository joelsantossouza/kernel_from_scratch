# =============================================================================
# IO DRIVERS
# =============================================================================
ROOT		:= $(shell git rev-parse --show-toplevel)

include $(ROOT)/config.mk

ASM_OBJS	:= io.asm.o
OBJS		:= $(ASM_OBJS)

.PHONY: all fclean

all: $(OBJS)

ASM_DEPS	:= $(ASM_OBJS:.o=.d)
-include	$(ASM_DEPS)
%.asm.o: %.asm
	$(AS) $(AFLAGS_ELF) $(INCLUDES) -MD $(@:.o=.d) $< -o $@

fclean:
	rm -f $(ASM_DEPS)
	rm -f $(OBJS)

re: fclean all
