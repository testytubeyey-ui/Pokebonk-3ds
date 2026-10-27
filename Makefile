# PokéBonk 2DS Mini - devkitPro/libctru project
# Build a .3dsx for Homebrew Launcher:
#   make
# A CIA requires an additional packaging step and appropriate metadata.

.SUFFIXES:
include $(DEVKITPRO)/libctru/base_rules

TARGET := pokebonk_2ds_mini
BUILD := build
SOURCES := source
INCLUDES := include

CFILES := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
export VPATH := $(foreach dir,$(SOURCES),$(CURDIR)/$(dir))
export INCLUDE := $(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) -I$(CURDIR)/$(BUILD)

.PHONY: all clean
all: $(TARGET).3dsx

$(TARGET).3dsx: $(TARGET).elf

$(TARGET).elf: $(CFILES)

clean:
	@rm -rf $(BUILD) $(TARGET).3dsx $(TARGET).elf
