# =============================================================================
# OS Build
# =============================================================================
ROOT_DIR			:= $(shell git rev-parse --show-toplevel)
include 			build.mk

IMGS				:= $(PARTITIONS_IMG) \
					   $(OS).img

KERNEL_TO_LOAD		?= $(KERNEL)
PART0_KERNEL		:= $(PART0_MNT)/boot/kernel

.PHONY: test

# OS Main Build
all: $(PART0_KERNEL) $(OS).img

# OS Test Build
test: KERNEL_TO_LOAD=$(TEST_KERNEL)
test: $(PART0_KERNEL) $(OS).img

$(PART0_KERNEL): $(PART0).img $(KERNEL_TO_LOAD).bin
	mkdir -p $(PART0_MNT)
	sudo mount $(PART0).img $(PART0_MNT)
	sudo mkdir -p $(dir $@)
	sudo $(CP) $(KERNEL_TO_LOAD).bin $@
	sudo umount $(PART0_MNT)

$(KERNEL).bin: build-kernel ;

# =============================================================================
# OS Simulation
# =============================================================================
BOOTLOADER_ADDR		:= $(CONFIG_BOOT_STAGE2_ADDR)
KERNEL_ADDR			:= $(CONFIG_KERNEL_ADDR)

.PHONY: debug-test debug gdb

debug-test: KERNEL_TO_LOAD=$(TEST_KERNEL)
debug-test: test gdb

debug: all gdb

gdb:
	gdb --tui \
		-ex "set confirm off" \
		-ex "add-symbol-file $(MBR).elf $(MBR_ADDR)" \
		-ex "add-symbol-file $(BOOTLOADER).elf $(BOOTLOADER_ADDR)" \
		-ex "add-symbol-file $(KERNEL_TO_LOAD).elf $(KERNEL_ADDR)" \
		-ex "target remote | $(EMU) $(EMUFLAGS) -hda $(OS).img"

# =============================================================================
# OS Clean up
# =============================================================================
fclean: fclean-boot fclean-kernel

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
