BLUEFRUITLE_DIR = bluefruitle_nrf51

SRC += $(BLUEFRUITLE_DIR)/suart.S \
	$(BLUEFRUITLE_DIR)/spi.c \
	$(BLUEFRUITLE_DIR)/sdep.c \
	$(BLUEFRUITLE_DIR)/bluefruitle_spi.c \
	$(BLUEFRUITLE_DIR)/bluefruitle_task.c \
	$(BLUEFRUITLE_DIR)/bluefruitle.c \
	$(BLUEFRUITLE_DIR)/battery.c \
	$(BLUEFRUITLE_DIR)/main.c

OPT_DEFS += -DPROTOCOL_BLUEFRUITLE

VPATH += $(BLUEFRUITLE_DIR)
