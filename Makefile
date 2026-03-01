# =============================================================================
# PROJECT
# =============================================================================
NAME			:= MyOS

CC				:= gcc
AS				:= nasm
CFLAGS			:=
AFLAGS			:=

SRCS_DIR		:= src
OBJS_DIR		:= build
INCLUDES_DIR	:= include

SRCS			:=
OBJS			:=
HEADERS			:=
INCLUDES		:= -I$(INCLUDES_DIR)

include build/boot/module.mk

# =============================================================================
# BUILD
# =============================================================================
all: $(NAME)

$(NAME): $(OBJS)

$(OBJS_DIR)/%.c.o: %(SRCS_DIR)/%.c $(HEADERS)

$(OBJS_DIR)/%.asm.o: $(SRCS_DIR)/%.asm $(HEADERS)
	$(AS) $(AFLAGS) $(INCLUDES) $< -o $@

# =============================================================================
# CLEAN
# =============================================================================
clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

# =============================================================================
# CHECKER
# =============================================================================
SHELL			:= /bin/bash
PYTHON			:= python3
PIP				:= pip3
PIP_PKG			:= python3-pip
NORMINETTE		:= norminette

CHECKERS_DIR	:= ./checkers
CHECKERS		:= $(addprefix $(CHECKERS_DIR)/, \
				   header_checker.py \
				   asm_bits_checker.py \
				   doc_checker.py \
)

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

