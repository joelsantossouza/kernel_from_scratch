# =============================================================================
# KERNEL
# =============================================================================
ROOT	:= $(shell git rev-parse --show-toplevel)

include $(ROOT)/config.mk

ASM_OBJS	:= kernel.asm.o

OBJS		:= $(ASM_OBJS)
BIN			:= kernel.bin
LINKER		:= linker.ld

.PHONY: all clean fclean re

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -T $(LINKER) $< -o $@

ASM_DEPS	:= $(ASM_OBJS:.o=.d)
-include	$(ASM_DEPS)
%.asm.o: %.asm
	$(AS) -f elf32 $(AFLAGS) $(INCLUDES) -MD $(@:.o=.d) $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(ASM_DEPS)
	rm -f $(BIN)

re: fclean all
