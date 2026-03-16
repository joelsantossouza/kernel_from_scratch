# =============================================================================
# FILE SYSTEMS
# =============================================================================
ROOT		:= $(shell git rev-parse --show-toplevel)

include $(ROOT)/config.mk

FAT16_DIR	:= fat/fat16

OBJS		:= $(FAT16_DIR)/fat16_interface.c.o \
			   $(FAT16_DIR)/fat16_probe.c.o \
			   vfs/vfs_mount.c.o \
			   vfs/vfs_partition.c.o \
			   vfs/vfs_table.c.o

C_OBJS		:= $(filter %.c.o, $(OBJS))

.PHONY: all clean fclean

all: $(OBJS)

# =============================================================================
# File System Build
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
