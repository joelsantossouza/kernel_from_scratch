# =============================================================================
# PROJECT
# =============================================================================
ROOT			:= $(shell git rev-parse --show-toplevel)

include $(ROOT)/config.mk

DISK_IMG		:= MyOS.img
DISK_SECTORS	:= 65536
SECTOR_SIZE		:= 512
MBR_GAP_SECTORS	:= 2047
PARTS_START		:= 2048

# DIRECTORIES
# ====================================
SRCS_DIR		:= src
BOOT_DIR		:= $(SRCS_DIR)/boot
KERNEL_DIR		:= $(SRCS_DIR)/kernel

# DEPENDENCIES
# ====================================
MBR_ELF			:= $(BOOT_DIR)/mbr.elf
MBR_ADDR		:= 0x7C00
BOOT_ELF		:= $(BOOT_DIR)/boot_stage2.elf
BOOT_ADDR		:= $(shell printf '0x%X' $$(( $(MBR_ADDR) + $(SECTOR_SIZE) )))

MBR_BIN			:= $(BOOT_DIR)/mbr.bin
BOOT_BIN		:= $(BOOT_DIR)/boot_stage2.bin
KERNEL_BIN		:= $(KERNEL_DIR)/kernel.bin

BINS			:= $(MBR_BIN) \
				   $(BOOT_BIN) \
				   $(KERNEL_BIN)

PART1_IMG		:= partition1.img
PART1_SECTORS	:= 32768

DD				:= dd bs=$(SECTOR_SIZE) conv=notrunc

QEMU_LOG		:= qemu.log

# =============================================================================
# BUILD
# =============================================================================
.PHONY: all part-deps fclean re build-boot build-kernel

all: part-deps $(DISK_IMG)

debug: all
	qemu-system-i386 -drive format=raw,file=$(DISK_IMG) -S -gdb tcp::1234 -d int,cpu_reset 2>$(QEMU_LOG) &
	gdb --tui \
		-ex "set confirm off" \
		-ex "add-symbol-file $(MBR_ELF) $(MBR_ADDR)" \
		-ex "add-symbol-file $(BOOT_ELF) $(BOOT_ADDR)" \
		-ex "target remote localhost:1234"

$(DISK_IMG): $(BINS) $(PART1_IMG)
	$(DD) if=/dev/zero		of=$(DISK_IMG) seek=0 count=$(DISK_SECTORS)
	$(DD) if=$(MBR_BIN)		of=$(DISK_IMG) seek=0 count=1					
	$(DD) if=$(BOOT_BIN)	of=$(DISK_IMG) seek=1 count=$(MBR_GAP_SECTORS)	
	$(DD) if=$(PART1_IMG)	of=$(DISK_IMG) seek=$(PARTS_START)

$(MBR_BIN) $(BOOT_BIN): build-boot
	$(MAKE) -C $(BOOT_DIR)

$(KERNEL_BIN): build-kernel
	$(MAKE) -C $(KERNEL_DIR)

$(PART1_IMG): $(KERNEL_BIN)
	$(DD) if=/dev/zero of=$(PART1_IMG) count=$(PART1_SECTORS)
	mkfs.fat -F 16 $(PART1_IMG)
	mmd -i $(PART1_IMG) ::/boot
	mcopy -i $(PART1_IMG) $(KERNEL_BIN) ::/boot/kernel

part-deps:
	@if ! which mmd >/dev/null 2>&1 || ! which mcopy >/dev/null 2>&1; then \
		echo "mmd/mcopy is not installed."; \
		read -p "Do you want to install it? [y/N] " ans; \
		if [ "$${ans^^}" = "Y" ]; then \
			sudo apt-get install -y mtools; \
		else \
			echo "Skipping mmd/mcopy."; \
		fi \
	fi

fclean:
	rm -f $(PART1_IMG)
	rm -f $(DISK_IMG)
	rm -f $(QEMU_LOG)
	$(MAKE) fclean -C $(BOOT_DIR)
	$(MAKE) fclean -C $(KERNEL_DIR)


re: fclean all

# =============================================================================
# CHECKER
# =============================================================================
SHELL			:= /bin/bash
PYTHON			:= python3
PIP				:= pip3
PIP_PKG			:= python3-pip
NORMINETTE		:= norminette

CHECKERS_DIR	:= checkers
CHECKERS		:= $(addprefix $(CHECKERS_DIR)/, \
				   header_checker.py \
				   asm_bits_checker.py \
				   doc_checker.py \
)

.PHONY: check check-deps

check: check-deps
	@for checker in $(CHECKERS); do \
		$(PYTHON) $$checker $(SRCS_DIR); \
		$(PYTHON) $$checker $(INCLUDES_DIR); \
	done
	$(NORMINETTE) $(SRCS_DIR)

check-deps:
	@if ! which $(PYTHON) > /dev/null 2>&1; then \
		echo "$(PYTHON) is not installed."; \
		read -p "Do you want to install it? [y/N] " ans; \
		if [ "$${ans^^}" = "Y" ]; then \
			sudo apt-get install -y $(PYTHON); \
		else \
			echo "Skipping $(PYTHON)."; \
		fi \
	fi
	@if ! which $(NORMINETTE) > /dev/null 2>&1; then \
		echo "$(NORMINETTE) is not installed."; \
		read -p "Do you want to install it? [y/N] " ans; \
		if [ "$${ans^^}" = "Y" ]; then \
			if ! which $(PIP) > /dev/null 2>&1; then \
				echo "$(PIP) not found, installing..."; \
				sudo apt-get install -y $(PIP_PKG); \
			fi; \
			pip3 install $(NORMINETTE); \
		else \
			echo "Skipping $(NORMINETTE)."; \
		fi \
	fi
