# =============================================================================
# BOILERPLATE MAKEFILE
# =============================================================================
.DEFAULT_GOAL		= all

# Directories
ROOT_DIR			:= $(shell git rev-parse --show-toplevel)

SRC_DIR				:= $(ROOT_DIR)/src
LIB_DIR				:= $(ROOT_DIR)/lib
INC_DIR				:= $(ROOT_DIR)/include
TEST_DIR			:= $(ROOT_DIR)/test
MNT_DIR				:= $(ROOT_DIR)/mnt

BOOT_DIR			:= $(SRC_DIR)/boot
KERNEL_DIR			:= $(SRC_DIR)/kernel
DRIVERS_DIR			:= $(SRC_DIR)/drivers
FS_DIR				:= $(SRC_DIR)/fs

IO_DIR				:= $(DRIVERS_DIR)/io
DISK_DIR			:= $(DRIVERS_DIR)/disk
VIDEO_DIR			:= $(DRIVERS_DIR)/video

STRING_DIR			:= $(LIB_DIR)/string

TEST_KERNEL_DIR		:= $(TEST_DIR)/kernel

# OS Components
OS_NAME				:= HavenOS
OS					:= ./$(OS_NAME)

MBR_NAME			:= mbr
MBR					:= $(BOOT_DIR)/$(MBR_NAME)

BOOTLOADER_NAME		:= boot_stage2
BOOTLOADER			:= $(BOOT_DIR)/$(BOOTLOADER_NAME)

KERNEL_NAME			:= kernel
KERNEL				:= $(KERNEL_DIR)/$(KERNEL_NAME)

TEST_KERNEL_NAME	:= test_kernel
TEST_KERNEL			:= $(TEST_KERNEL_DIR)/$(TEST_KERNEL_NAME)

PART0_NAME			:= part0
PART0_MNT			:= $(MNT_DIR)/$(PART0_NAME)
PART0				:= ./$(PART0_NAME)

PART1_NAME			:= part1
PART1_MNT			:= $(MNT_DIR)/$(PART1_NAME)
PART1				:= ./$(PART1_NAME)

PART2_NAME			:= part2
PART2_MNT			:= $(MNT_DIR)/$(PART2_NAME)
PART2				:= ./$(PART2_NAME)

PART3_NAME			:= part3
PART3_MNT			:= $(MNT_DIR)/$(PART3_NAME)
PART3				:= ./$(PART3_NAME)

PARTITIONS_IMG		:= $(PART0).img \
					   $(PART1).img \
					   $(PART2).img \
					   $(PART3).img

# OS Constants
include							$(ROOT_DIR)/.config
DISK_SECTOR_BYTES				:= 512
MBR_SECTORS						:= 1
MBR_ADDR						:= 0x7C00
BOOTLOADER_SECTORS				:= $(CONFIG_BOOT_STAGE2_SECTORS)

BOOTLOADER_START				:= $(shell printf "%d" $$(( \
								   $(CONFIG_BOOT_STAGE2_SECTOR) - 1 \
)))
CONFIG_DISK_PART0_SECTORS		:= $(shell printf "%d" $$(( \
								   $(CONFIG_DISK_PART0_END) - $(CONFIG_DISK_PART0_START) \
)))
CONFIG_DISK_PART1_SECTORS		:= $(shell printf "%d" $$(( \
								   $(CONFIG_DISK_PART1_END) - $(CONFIG_DISK_PART1_START) \
)))
CONFIG_DISK_PART2_SECTORS		:= $(shell printf "%d" $$(( \
								   $(CONFIG_DISK_PART2_END) - $(CONFIG_DISK_PART2_START) \
)))
CONFIG_DISK_PART3_SECTORS		:= $(shell printf "%d" $$(( \
								   $(CONFIG_DISK_PART3_END) - $(CONFIG_DISK_PART3_START) \
)))
CONFIG_VDL_CACHE_ENTRY_BYTES	:= $(shell printf "%d" $$(( \
								   $(CONFIG_VDL_CACHE_ENTRY_SECTORS) * $(DISK_SECTOR_BYTES) \
)))

# Tools
CC					:= gcc
AS					:= nasm
LD					:= ld
DD					:= dd
MKFS				:= mkfs
CP					:= cp
EMU					:= qemu-system-i386

# Flags
CFLAGS				:= -m32 -fno-pic -fno-pie -Wall -Wextra -Werror -g -ffreestanding -nostdlib -fno-builtin
AFLAGS				:= -w+all -w+error -f elf32 -g -F dwarf
LDFLAGS				:= -m elf_i386 -nostdlib --no-undefined
DDFLAGS				:= bs=$(DISK_SECTOR_BYTES) conv=notrunc
EMUFLAGS			:= -S -gdb stdio
INCLUDE				:= -I$(INC_DIR) -I$(LIB_DIR)

# Commands
define assert_options
$(if $(filter $(1),$($(2))),,\
	$(error $(2) must be one of: $(1) | current value = $($(2))))
endef

define assert_range
$(if $(shell \
    val=$$(printf "%d" $($(3))); \
    min=$$(printf "%d" $(1)); \
    max=$$(printf "%d" $(2)); \
    [ $$val -ge $$min ] && [ $$val -le $$max ] && echo OK),,\
    $(error $(3) must be between $(1) and $(2) | current value = $($(3))))
endef

define assert_range_span
$(if $(shell \
    start=$$(printf "%d" $($(3))); \
    count=$$(printf "%d" $(4)); \
    size=$$(printf "%d" $(5)); \
    min=$$(printf "%d" $(1)); \
    max=$$(printf "%d" $(2)); \
    end=$$((start + count * size)); \
    [ $$start -ge $$min ] && [ $$end -le $$max ] && echo OK),,\
    $(error $(3) span [$($(3)), \
$(shell printf "0x%x" $$(( $$(printf "%d" $($(3))) + $$(printf "%d" $(4)) * $$(printf "%d" $(5)) )))] \
must fit between $(1) and $(2)))
endef

define assert_align
$(if $(filter-out 0,$(shell printf "%d" $$(( $($(1)) % $(2) )))), \
    $(error $(1) is not aligned to $(2) | current value = $($(1))),)
endef

define append_config_header
	echo "#define $(1) $($(1))" >> $(2)
endef

# =============================================================================
# OS Configuration
# =============================================================================
CONFIG_MENU			:= menuconfig
CONFIG_HEADERGEN	:= genconfig
CONFIG_SCRIPT		:= $(ROOT_DIR)/Kconfig
CONFIG_FILE			:= $(ROOT_DIR)/.config
CONFIG_HEADER		:= $(INC_DIR)/autoconfig

.PHONY: menuconfig config genconfig-h genconfig-inc

menuconfig: $(CONFIG_SCRIPT)
	cd $(ROOT_DIR) && $(CONFIG_MENU) $<

config: $(CONFIG_FILE)

genconfig-h: $(CONFIG_HEADER).h

genconfig-inc: $(CONFIG_HEADER).inc

$(CONFIG_FILE): $(CONFIG_SCRIPT)
	cd $(ROOT_DIR) && $(CONFIG_MENU) $<

$(CONFIG_HEADER).h: $(CONFIG_SCRIPT) $(CONFIG_FILE)
	cd $(ROOT_DIR) && $(CONFIG_HEADERGEN) --header-path $@ $<
	$(call append_config_header,CONFIG_DISK_PART0_SECTORS,$@)
	$(call append_config_header,CONFIG_DISK_PART1_SECTORS,$@)
	$(call append_config_header,CONFIG_DISK_PART2_SECTORS,$@)
	$(call append_config_header,CONFIG_DISK_PART3_SECTORS,$@)
	$(call append_config_header,CONFIG_VDL_CACHE_ENTRY_BYTES,$@)

$(CONFIG_HEADER).inc: $(CONFIG_HEADER).h
	sed "s/#define/%define/g" $< > $@

# =============================================================================
# Build
# =============================================================================
.PHONY: all clean fclean re build-boot build-kernel fclean-boot fclean-kernel
.SECONDARY:

# C Objects
%.c.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE) -MMD -MP -c $< -o $@

# Assembly Objects
%.asm.o: %.asm
	$(AS) $(AFLAGS) $(INCLUDE) -MD $(@:.o=.d) $< -o $@

# Binary
%.bin:
	$(LD) $(LDFLAGS) --oformat binary $^ -o $@

# Elf
%.elf:
	$(LD) $(LDFLAGS) --oformat elf32-i386 $^ -o $@

# Subsystem
build-boot:
	$(MAKE) -C $(BOOT_DIR)

build-kernel:
	$(MAKE) -C $(KERNEL_DIR)

# Partition Images
$(PART0).img:
	$(DD) $(DDFLAGS) if=/dev/zero of=$@ count=$(CONFIG_DISK_PART0_SECTORS)
	$(MKFS) -t $(CONFIG_DISK_PART0_FS_FAMILY) -F $(CONFIG_DISK_PART0_FS_VERSION) $@

$(PART1).img:
	$(DD) $(DDFLAGS) if=/dev/zero of=$@ count=$(CONFIG_DISK_PART1_SECTORS)
	$(MKFS) -t $(CONFIG_DISK_PART1_FS_FAMILY) -F $(CONFIG_DISK_PART1_FS_VERSION) $@

$(PART2).img:
	$(DD) $(DDFLAGS) if=/dev/zero of=$@ count=$(CONFIG_DISK_PART2_SECTORS)
	$(MKFS) -t $(CONFIG_DISK_PART2_FS_FAMILY) -F $(CONFIG_DISK_PART2_FS_VERSION) $@

$(PART3).img:
	$(DD) $(DDFLAGS) if=/dev/zero of=$@ count=$(CONFIG_DISK_PART3_SECTORS)
	$(MKFS) -t $(CONFIG_DISK_PART3_FS_FAMILY) -F $(CONFIG_DISK_PART3_FS_VERSION) $@

# OS Image
$(OS).img: build-boot $(PARTITIONS_IMG)
	$(DD) $(DDFLAGS) if=$(MBR).bin		  of=$@ seek=0									count=$(MBR_SECTORS)
	$(DD) $(DDFLAGS) if=$(BOOTLOADER).bin of=$@ seek=$(BOOTLOADER_START)				count=$(BOOTLOADER_SECTORS)
	$(DD) $(DDFLAGS) if=$(PART0).img	  of=$@ seek=$$(( $(CONFIG_DISK_PART0_START) ))	count=$(CONFIG_DISK_PART0_SECTORS)
	$(DD) $(DDFLAGS) if=$(PART1).img	  of=$@ seek=$$(( $(CONFIG_DISK_PART1_START) ))	count=$(CONFIG_DISK_PART1_SECTORS)
	$(DD) $(DDFLAGS) if=$(PART2).img	  of=$@ seek=$$(( $(CONFIG_DISK_PART2_START) ))	count=$(CONFIG_DISK_PART2_SECTORS)
	$(DD) $(DDFLAGS) if=$(PART3).img	  of=$@ seek=$$(( $(CONFIG_DISK_PART3_START) ))	count=$(CONFIG_DISK_PART3_SECTORS)

# =============================================================================
# Clean up
# =============================================================================
clean:
	rm -f $(DEPS)
	rm -f $(OBJS)

fclean: clean
	rm -f $(BINS)
	rm -f $(ELFS)
	rm -f $(IMGS)

fclean-boot:
	$(MAKE) fclean -C $(BOOT_DIR)

fclean-kernel:
	$(MAKE) fclean -C $(KERNEL_DIR)

re: fclean all
