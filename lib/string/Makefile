# =============================================================================
# STRING LIB
# =============================================================================
ROOT		:= $(shell git rev-parse --show-toplevel)

include $(ROOT)/config.mk

OBJS		:= memcpy.asm.o \
			   memcmp.asm.o \
			   strlen.asm.o \
			   strcmp.asm.o

ASM_OBJS	:= $(filter %.asm.o, $(OBJS))

.PHONY: all clean fclean

all: $(OBJS)

# =============================================================================
# String Lib Build
# =============================================================================
ASM_DEPS	:= $(ASM_OBJS:.o=.d)
-include	$(ASM_DEPS)
%.asm.o: %.asm
	$(AS) $(AFLAGS_ELF) $(INCLUDES) -MD $(@:.o=.d) $< -o $@

# =============================================================================
# Clean up
# =============================================================================
clean:
	rm -f $(OBJS)
	rm -f $(ASM_DEPS)

fclean: clean

re: fclean all
