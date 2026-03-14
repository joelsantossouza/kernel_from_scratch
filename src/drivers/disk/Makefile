# =============================================================================
# DISK DRIVERS
# =============================================================================
ROOT		:= $(shell git rev-parse --show-toplevel)

include $(ROOT)/config.mk

OBJS		:= ata/disk_ata.c.o \
			   ata/disk_ata_stat.c.o \
			   vdl/disk_vdl.c.o

C_OBJS		:= $(filter %.c.o, $(OBJS))

.PHONY: all clean fclean

all: $(OBJS)

# =============================================================================
# Disk Drivers Build
# =============================================================================
C_DEPS		:= $(C_OBJS:.o=.d)
-include	$(C_DEPS)
%.c.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@

# =============================================================================
# Clean up
# =============================================================================
clean:
	rm -f $(OBJS)
	rm -f $(C_DEPS)

fclean: clean

re: fclean all
