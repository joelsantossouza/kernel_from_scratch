# =============================================================================
# BOOT
# =============================================================================
ROOT		:= $(shell git rev-parse --show-toplevel)

include $(ROOT)/config.mk

IO_DIR		:= ../drivers/io
DISK_DIR	:= ../drivers/disk

IO_OBJS		:= $(IO_DIR)/io.asm.o
DISK_OBJS	:= $(DISK_DIR)/ata/disk_ata.c.o
BOOT_OBJS	:= boot_stage2.asm.o $(IO_OBJS) $(DISK_OBJS)

BOOT_BIN	:= boot_stage2.bin
BOOT_ELF	:= boot_stage2.elf

BINS		:= mbr.bin $(BOOT_BIN)
ELFS		:= mbr.elf $(BOOT_ELF)

LINKER		:= linker.ld

.PHONY: all fclean re build-io build-disk

all: $(BINS) $(ELFS)

$(BOOT_BIN): $(BOOT_OBJS)
	$(CC) $(LDFLAGS) -T $(LINKER) -Wl,--oformat,binary $^ -o $@

$(BOOT_ELF): $(BOOT_OBJS)
	$(CC) $(LDFLAGS) -T $(LINKER) $^ -o $@

$(IO_OBJS): build-io
	$(MAKE) -C $(IO_DIR)

$(DISK_OBJS): build-disk
	$(MAKE) -C $(DISK_DIR)

%.bin: %.asm $(HEADERS)
	$(AS) $(AFLAGS_BIN) $(INCLUDES) $< -o $@

%.elf: %.asm $(HEADERS)
	$(AS) $(AFLAGS_ELF) $(INCLUDES) $< -o $@

%.asm.o: %.asm $(HEADERS)
	$(AS) $(AFLAGS_ELF) $(INCLUDES) $< -o $@

fclean:
	rm -f $(BINS)
	rm -f $(ELFS)
	rm -f $(BOOT_OBJS)
	$(MAKE) fclean -C $(IO_DIR)
	$(MAKE) fclean -C $(DISK_DIR)

re: fclean all
