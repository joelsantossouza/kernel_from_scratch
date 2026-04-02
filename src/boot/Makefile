# =============================================================================
# BOOT
# =============================================================================
ROOT_DIR		:= $(shell git rev-parse --show-toplevel)
include $(ROOT_DIR)/build.mk
include .config

MBR_SRCS		:= mbr.asm

BOOTLOADER_SRCS	:= boot_stage2.asm \
				   $(DISK_DIR)/ata/disk_ata.c \
				   $(DISK_DIR)/ata/disk_ata_stat.c \
				   $(DISK_DIR)/vdl/disk_vdl.c \
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
LDFLAGS			+= -T $(BOOTLOADER_NAME).ld -defsym=BOOT_STAGE2_ADDR=$(BOOT_STAGE2_ADDR)

-include	$(DEPS)

all: $(BINS) $(ELFS)

$(MBR_NAME).bin: $(MBR_SRCS)
	$(AS) $(INCLUDES) -f bin $< -o $@

$(MBR_NAME).elf: $(MBR_SRCS)
	$(AS) $(INCLUDES) -f elf32 $< -o $@

$(BOOTLOADER_NAME).bin $(BOOTLOADER_NAME).elf: $(BOOTLOADER_OBJS)

assert:
	$(call assert_options,0x0 0x80,MBR_PART0_BOOT_ATTR)
	$(call assert_options,0x0 0x80,MBR_PART1_BOOT_ATTR)
	$(call assert_options,0x0 0x80,MBR_PART2_BOOT_ATTR)
	$(call assert_options,0x0 0x80,MBR_PART3_BOOT_ATTR)
	$(call assert_range,0x0,0xFF,MBR_PART0_TYPECODE)
	$(call assert_range,0x0,0xFF,MBR_PART1_TYPECODE)
	$(call assert_range,0x0,0xFF,MBR_PART2_TYPECODE)
	$(call assert_range,0x0,0xFF,MBR_PART3_TYPECODE)

	$(call assert_range,0x7E00,0x70000,BOOT_STAGE2_ADDR)
	$(call assert_range,1,127,BOOT_STAGE2_SECTORS)
	$(call assert_range,2,63,BOOT_STAGE2_SECTOR)
	$(call assert_range,0,1023,BOOT_STAGE2_CYLINDER)
	$(call assert_range,0,255,BOOT_STAGE2_HEAD)
