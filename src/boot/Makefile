# =============================================================================
# BOOT
# =============================================================================
ROOT_DIR		:= $(shell git rev-parse --show-toplevel)
include $(ROOT_DIR)/build.mk

MBR_SRCS		:= mbr.asm

BOOTLOADER_SRCS	:= boot_stage2.asm \
				   $(DISK_DIR)/ata/ata_driver.c \
				   $(DISK_DIR)/ata/ata_io.c \
				   $(DISK_DIR)/ata/ata_read.c \
				   $(DISK_DIR)/ata/ata_stat.c \
				   $(DISK_DIR)/vdl/vdl_disks.c \
				   $(DISK_DIR)/vdl/vdl_read.c \
				   $(DISK_DIR)/vdl/vdl_cache.c \
				   $(IO_DIR)/io.asm \
				   $(FS_DIR)/vfs/vfs_mount_table.c \
				   $(FS_DIR)/vfs/vfs_partition.c \
				   $(FS_DIR)/vfs/vfs_table.c \
				   $(FS_DIR)/fat/fat_cluster_read.c \
				   $(FS_DIR)/fat/fat_file_name.c \
				   $(FS_DIR)/fat/fat_dir_open.c \
				   $(FS_DIR)/fat/fat_file_open.c \
				   $(FS_DIR)/fat/fat16/fat16_interface.c \
				   $(FS_DIR)/fat/fat16/fat16_probe.c \
				   $(FS_DIR)/fat/fat16/fat16_dir_open.c \
				   $(STRING_DIR)/memcmp.asm \
				   $(STRING_DIR)/memcpy.asm

BOOTLOADER_OBJS	:= $(addsuffix .o, $(BOOTLOADER_SRCS))
BINS			:= $(MBR_NAME).bin $(BOOTLOADER_NAME).bin
ELFS			:= $(MBR_NAME).elf $(BOOTLOADER_NAME).elf
OBJS			:= $(BOOTLOADER_OBJS)
DEPS			:= $(OBJS:.o=.d)
LDFLAGS			+= -T $(BOOTLOADER_NAME).ld -defsym=CONFIG_BOOT_STAGE2_ADDR=$(CONFIG_BOOT_STAGE2_ADDR)

-include	$(DEPS)

all: $(CONFIG_HEADER).inc $(BINS) $(ELFS)

$(MBR_NAME).bin: $(MBR_SRCS)
	$(AS) $(INCLUDE) -f bin $< -o $@

$(MBR_NAME).elf: $(MBR_SRCS)
	$(AS) $(INCLUDE) -f elf32 $< -o $@

$(BOOTLOADER_NAME).bin $(BOOTLOADER_NAME).elf: $(BOOTLOADER_OBJS)
