# =============================================================================
# KERNEL
# =============================================================================
ROOT	:= $(shell git rev-parse --show-toplevel)

include $(ROOT)/config.mk

BIN		:= kernel.bin
OBJS	:= kernel.asm.o
LINKER	:= linker.ld

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -T $(LINKER) $< -o $@

%.asm.o: %.asm $(HEADERS)
	$(AS) -f elf32 $(AFLAGS) $(INCLUDES) $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(BIN)

re: fclean all
