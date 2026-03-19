# =============================================================================
# BOOT
# =============================================================================
ROOT		:= $(shell git rev-parse --show-toplevel)
include $(ROOT)/config.mk

MBR_BIN		:= mbr.bin
MBR_ELF		:= mbr.elf
MBR_OBJS	:= mbr.asm.o

BOOT_BIN	:= boot_stage2.bin
BOOT_ELF	:= boot_stage2.elf
BOOT_OBJS	:= boot_stage2.asm.o \
			   $(DISK_DIR)/ata/disk_ata.c.o \
			   $(DISK_DIR)/ata/disk_ata_stat.c.o \
			   $(DISK_DIR)/vdl/disk_vdl.c.o \
			   $(IO_DIR)/io.asm.o \
			   $(FS_DIR)/vfs/vfs_mount_table.c.o \
			   $(FS_DIR)/vfs/vfs_partition.c.o \
			   $(FS_DIR)/vfs/vfs_table.c.o \
			   $(FS_DIR)/fat/fat16/fat16_interface.c.o \
			   $(FS_DIR)/fat/fat16/fat16_probe.c.o \
			   $(STRING_DIR)/memcmp.asm.o \
			   $(STRING_DIR)/memcpy.asm.o

BINS		:= $(MBR_BIN) $(BOOT_BIN)
ELFS		:= $(MBR_ELF) $(BOOT_ELF)
OBJS		:= $(MBR_OBJS) $(BOOT_OBJS)
DEPS		:= $(OBJS:.o=.d)

-include	$(DEPS)

all: $(BINS) $(ELFS)

$(MBR_BIN) $(MBR_ELF): $(MBR_OBJS)
$(BOOT_BIN) $(BOOT_ELF): $(BOOT_OBJS)
