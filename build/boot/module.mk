THIS		:= boot

SRCS		+= $(addprefix $(SRCS_DIR)/$(THIS)/, \
				mbr.asm \
)

OBJS		+= $(patsubst $(SRCS_DIR)/%, $(OBJS_DIR)/%.o, $(SRCS))

HEADER		+= $(addprefix $(INCLUDES_DIR)/$(THIS), \
				config.inc \
)
