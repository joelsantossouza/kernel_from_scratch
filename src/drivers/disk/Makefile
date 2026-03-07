# =============================================================================
# DISK DRIVERS
# =============================================================================
ROOT		:= $(shell git rev-parse --show-toplevel)

include $(ROOT)/config.mk

ATA_OBJS	:= ata/disk_ata.c.o

C_OBJS		:= $(ATA_OBJS)
OBJS		:= $(C_OBJS)

.PHONY: all fclean

all: $(OBJS)

C_DEPS		:= $(C_OBJS:.o=.d)
-include	$(C_DEPS)
%.c.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@

fclean:
	rm -f $(OBJS)
	rm -f $(C_DEPS)

re: fclean all
