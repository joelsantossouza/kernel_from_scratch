# =============================================================================
# BOOT
# =============================================================================
ROOT		:= $(shell git rev-parse --show-toplevel)
include $(ROOT)/config.mk

MBR_NAME	:= mbr
MBR_SRCS	:= mbr.asm
MBR_OBJS	:= $(addsuffix .o, $(MBR_SRCS))

BOOT_NAME	:= boot_stage2
BOOT_SRCS	:= boot_stage2.asm \
			   $(DISK_DIR)/ata/disk_ata.c \
			   $(DISK_DIR)/ata/disk_ata_stat.c \
			   $(DISK_DIR)/vdl/disk_vdl.c \
			   $(IO_DIR)/io.asm \
			   $(FS_DIR)/vfs/vfs_mount_table.c \
			   $(FS_DIR)/vfs/vfs_partition.c \
			   $(FS_DIR)/vfs/vfs_table.c \
			   $(FS_DIR)/fat/fat_cluster_read.c \
			   $(FS_DIR)/fat/fat16/fat16_interface.c \
			   $(FS_DIR)/fat/fat16/fat16_probe.c \
			   $(STRING_DIR)/memcmp.asm \
			   $(STRING_DIR)/memcpy.asm

BOOT_OBJS	:= $(addsuffix .o, $(BOOT_SRCS))

BINS		:= $(MBR_NAME).bin $(BOOT_NAME).bin
ELFS		:= $(MBR_NAME).elf $(BOOT_NAME).elf
OBJS		:= $(MBR_OBJS) $(BOOT_OBJS)
DEPS		:= $(OBJS:.o=.d)

-include	$(DEPS)

all: $(BINS) $(ELFS)

$(MBR_NAME).bin $(MBR_NAME).elf: $(MBR_OBJS)
$(BOOT_NAME).bin $(BOOT_NAME).elf: $(BOOT_OBJS)
