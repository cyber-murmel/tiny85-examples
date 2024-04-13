TARGET ?= blink
PLATFORM ?= t85_default
TTY_PORT ?= /dev/ttyACM0

CONFIGPATH = micronucleus/firmware/configuration/$(PLATFORM)
include $(CONFIGPATH)/Makefile.inc
include ./mkenv.mk

CROSS_COMPILE ?= avr-
MICRONUCLEUS ?= micronucleus
PROGRAMMER ?= -c USBasp
AVRDUDE = avrdude $(PROGRAMMER) -p $(DEVICE)
PYTHON ?= python3

include src/$(TARGET)/Makefile.inc

DIRS = $(sort \
	lib/ \
	src/$(TARGET)/ \
	$(TARGET_DIRS) \
	$(CONFIGPATH) \
)

INC += $(addprefix -I,$(DIRS))

CFLAGS_AVR = -DF_CPU=$(F_CPU) -mmcu=$(DEVICE) -DBOOTLOADER_ADDRESS=0x$(BOOTLOADER_ADDRESS)
CFLAGS = $(INC) $(CFLAGS_AVR) -std=c99 -Os -g2 -nostartfiles -ffunction-sections -fdata-sections -fpack-struct -fno-inline-small-functions -fno-move-loop-invariants -fno-tree-scev-cprop -Wno-narrowing
CFLAGS += $(TARGET_FLAGS)
LDFLAGS = $(CFLAGS_AVR) -Wl,--relax,--gc-sections,-Map=$(BUILD)/$(TARGET).map

# Flags for optional C++ source code
CXXFLAGS += $(filter-out -std=c99,$(CFLAGS))

LIBS = $(TARGET_LIBS)

SRC_C += $(addprefix src/$(TARGET)/, $(TARGET_SRC_C))
SRC_CXX += $(addprefix src/$(TARGET)/, $(TARGET_SRC_CXX))
SRC_S += $(addprefix src/$(TARGET)/, $(TARGET_SRC_S))

OBJ += $(addprefix $(BUILD)/, $(SRC_C:.c=.o))
OBJ += $(addprefix $(BUILD)/, $(SRC_CXX:.cpp=.o))
OBJ += $(addprefix $(BUILD)/, $(SRC_S:.S=.o))

all: $(BUILD)/$(TARGET).hex

$(BUILD)/%.hex: $(BUILD)/%.elf
	$(Q)$(OBJCOPY) -j .text -j .data --output-target=ihex $^ $@

$(BUILD)/$(TARGET).elf: $(OBJ)
	$(ECHO) "LINK $@"
	$(Q)$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)
	$(Q)$(SIZE) $@

flash: $(BUILD)/$(TARGET).hex
	$(Q)$(AVRDUDE) -U flash:w:$^:i
.PHONY: flash

readflash:
	$(Q)$(AVRDUDE) -U flash:r:read.hex:i -B 20
.PHONY: readflash

fuse:
	$(Q)$(AVRDUDE) $(FUSEOPT) -B 20
.PHONY: fuse

read_fuses:
	$(Q)$(AVRDUDE) -B 20
.PHONY: read_fuses

upload: $(BUILD)/$(TARGET).hex
	$(Q)$(MICRONUCLEUS) --run $^
.PHONY: upload

term: $(BUILD)/$(TARGET).hex
	$(Q)$(PYTHON) -m serial.tools.miniterm --exit-char 24 --raw $(TTY_PORT)
.PHONY: upload

format:
	$(Q)find src/ -type f -regex '.*\.\(h\|c\|cpp\)$$' -exec clang-format -i -style=WebKit {} +
.PHONY: format

include ./mkrules.mk
