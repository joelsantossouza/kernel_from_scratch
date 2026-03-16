# =============================================================================
# BOOT
# =============================================================================
ROOT		:= $(shell git rev-parse --show-toplevel)

include $(ROOT)/config.mk

IO_DIR		:= ../drivers/io
DISK_DIR	:= ../drivers/disk
STRING_DIR	:= ../../lib/string

BOOT_OBJS	:= boot_stage2.asm.o \
			   $(IO_DIR)/io.asm.o \
			   $(DISK_DIR)/ata/disk_ata.c.o \
			   $(DISK_DIR)/ata/disk_ata_stat.c.o \
			   $(DISK_DIR)/vdl/disk_vdl.c.o \
			   $(STRING_DIR)/memcpy.asm.o \
			   $(STRING_DIR)/strlen.asm.o \
			   $(STRING_DIR)/strcmp.asm.o

ASM_OBJS	:= $(filter %.asm.o, $(BOOT_OBJS))

BOOT_BIN	:= boot_stage2.bin
BOOT_ELF	:= boot_stage2.elf

BINS		:= mbr.bin $(BOOT_BIN)
ELFS		:= mbr.elf $(BOOT_ELF)

LINKER		:= linker.ld

.PHONY: all clean fclean re build-io build-disk build-libstring

all: build-io build-disk build-libstring $(BINS) $(ELFS)

# =============================================================================
# Boot Stage2 Build
# =============================================================================
$(BOOT_BIN): $(BOOT_OBJS)
	$(CC) $(LDFLAGS) -T $(LINKER) -Wl,--oformat,binary $^ -o $@

$(BOOT_ELF): $(BOOT_OBJS)
	$(CC) $(LDFLAGS) -T $(LINKER) $^ -o $@

# =============================================================================
# Boot Stage2 Objects Build
# =============================================================================
build-io:
	$(MAKE) -C $(IO_DIR)

build-disk:
	$(MAKE) -C $(DISK_DIR)

build-libstring:
	$(MAKE) -C $(STRING_DIR)

# =============================================================================
# General builds
# =============================================================================
%.bin: %.asm
	$(AS) $(AFLAGS_BIN) $(INCLUDES) $< -o $@

%.elf: %.asm
	$(AS) $(AFLAGS_ELF) $(INCLUDES) $< -o $@

ASM_DEPS	:= $(ASM_OBJS:.o=.d)
-include	$(ASM_DEPS)
%.asm.o: %.asm
	$(AS) $(AFLAGS_ELF) $(INCLUDES) -MD $(@:.o=.d) -MP $< -o $@

# =============================================================================
# Clean up
# =============================================================================
clean:
	rm -f $(ASM_DEPS)
	rm -f $(BOOT_OBJS)

fclean: clean
	rm -f $(BINS)
	rm -f $(ELFS)
	$(MAKE) fclean -C $(IO_DIR)
	$(MAKE) fclean -C $(DISK_DIR)
	$(MAKE) fclean -C $(STRING_DIR)

re: fclean all
