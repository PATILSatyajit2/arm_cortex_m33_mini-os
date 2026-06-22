# ============================================================
# Makefile for mini-os on MCXN236 (Cortex-M33)
# ============================================================
COMPILER_INSTALL_DIR  := C:\NXP\MCUXpressoIDE_25.6.136\ide\tools\bin

CC      = $(COMPILER_INSTALL_DIR)\arm-none-eabi-gcc.exe
AS      = $(COMPILER_INSTALL_DIR)\arm-none-eabi-gcc
LD      = $(COMPILER_INSTALL_DIR)\arm-none-eabi-gcc
OBJCOPY = $(COMPILER_INSTALL_DIR)\arm-none-eabi-objcopy
SIZE    = $(COMPILER_INSTALL_DIR)\arm-none-eabi-size

# ----- CPU flags for Cortex-M33 with single-precision FPU -----
CPU_FLAGS = -mcpu=cortex-m33 -mthumb -mfpu=fpv5-sp-d16 -mfloat-abi=hard

# ----- Compiler flags -----
CFLAGS  = $(CPU_FLAGS)          \
          -O0 -g3               \
          -Wall -Wextra         \
          -ffunction-sections   \
          -fdata-sections       \
          -std=c11              \
          -ffixed-r4            \
          -Isrc

# ----- Assembler flags -----
ASFLAGS = $(CPU_FLAGS) -g3

# ----- Linker flags -----
LDFLAGS = -nostdlib -Xlinker -no-warn-rwx-segments -Xlinker --gc-sections -Xlinker -print-memory-usage -Xlinker --sort-section=alignment -Xlinker --cref -mcpu=cortex-m33 -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -T linker.ld -Wl,-Map=build/output.map -Wl,--gc-sections $(CPU_FLAGS) -specs=nano.specs -lc -lm -nostartfiles

# ----- Sources -----
C_SRCS   := src/main.c
ASM_SRCS := src/startup.s
INCLUDES = -Iinc
# ----- Objects -----
C_OBJS   = $(patsubst src/%.c,   build/%.o, $(C_SRCS))
ASM_OBJS = $(patsubst src/%.s,   build/%.o, $(ASM_SRCS))
OBJS     = $(C_OBJS) $(ASM_OBJS)
TARGET   = build/mini-os
# ------------------------------------------------------------
# Default target
# ------------------------------------------------------------
all: $(TARGET).elf
# ------------------------------------------------------------
# Create build directory
# ------------------------------------------------------------
build:
	if not exist build mkdir build

# ------------------------------------------------------------
# Compile C files
# ------------------------------------------------------------
build/%.o: src/%.c | build
	@echo ==========================================================================================
	@echo                            Compiling $<
	@echo ==========================================================================================
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# ------------------------------------------------------------
# Assemble startup
# ------------------------------------------------------------
build/%.o: src/%.s | build
	@echo ==========================================================================================
	@echo                            Assembling $<
	@echo ==========================================================================================
	$(AS) $(ASFLAGS) -c $< -o $@

# ------------------------------------------------------------
# Link ELF
# ------------------------------------------------------------
$(TARGET).elf: $(OBJS)
	@echo ==========================================================================================
	@echo                            Linking $(TARGET).elf
	@echo ==========================================================================================
	$(LD) $(OBJS) $(LDFLAGS) -o $@
	$(SIZE) $@
# ------------------------------------------------------------
# Generate BIN
# ------------------------------------------------------------
bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $(TARGET).bin

# ------------------------------------------------------------
# Clean
# ------------------------------------------------------------
clean:
	if exist build rmdir /s /q build

.PHONY: all clean bin