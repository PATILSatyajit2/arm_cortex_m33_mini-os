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
          -Isrc                 \
          -Isecure              \
          -Inonsecure           \
          -mcmse                
# ----- Assembler flags -----
ASFLAGS = $(CPU_FLAGS) -g3

# ----- Linker flags -----
LDFLAGS = -nostdlib -Xlinker -no-warn-rwx-segments -Xlinker --gc-sections -Xlinker -print-memory-usage -Xlinker --sort-section=alignment -Xlinker --cref -mcpu=cortex-m33 -mcmse -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -T linker/secure.ld -Wl,-Map=build/output.map -Wl,--gc-sections $(CPU_FLAGS) -specs=nano.specs -nostdlib -lc -lm -lgcc -nostartfiles
# LDFLAGS = -nostdlib -Xlinker -no-warn-rwx-segments -Xlinker --gc-sections -Xlinker -print-memory-usage -Xlinker --sort-section=alignment -Xlinker --cref -mcpu=cortex-m33 -mcmse -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -T linker/secure.ld -Wl,-Map=build/output.map -Wl,--gc-sections $(CPU_FLAGS) -specs=nano.specs -lc -lm -nostartfiles

# ----- Sources -----
C_SRCS   := source/secure/sau_init.c\
            source/secure/nsc_api.c\
            source/secure/main_s.c
            
ASM_SRCS := source/secure/startup_s.s
INCLUDES_S = -Isecure
# ----- Secure Objects -----
C_OBJS_S   = $(patsubst source/secure/%.c,   build/%.o, $(C_SRCS))
ASM_OBJS_S = $(patsubst source/secure/%.s,   build/%.o, $(ASM_SRCS))
OBJS_S     = $(C_OBJS_S) $(ASM_OBJS_S)
# ----- Non Secure Objects -----
LDFLAGS_NS = $(CPU) -T linker/nonsecure.ld -Wl,--gc-sections
LDFLAGS_NS += -Wl,-Map=build/nonsecure.map -specs=nano.specs -nostartfiles

INCLUDES_NS = -Isecure  -Inonsecure
NS_CSRCS = source/nonsecure/main_ns.c
NS_ASRCS = source/nonsecure/startup_ns.s
NS_COBJS = $(patsubst source/nonsecure/%.c, build/%.o, $(NS_CSRCS))
NS_AOBJS = $(patsubst source/nonsecure/%.s, build/%.o, $(NS_ASRCS))
OBJS_NS = $(NS_COBJS) $(NS_AOBJS)
# ----- Targets -----
TARGET_S   = build/secure
TARGET_NS   = build/nonsecure
# ------------------------------------------------------------
# Default target
# ------------------------------------------------------------
all: $(TARGET_S).elf $(TARGET_NS).elf
# ------------------------------------------------------------
# Create build directory
# ------------------------------------------------------------
build:
	if not exist build mkdir build

# ------------------------------------------------------------
# Compile C files
# ------------------------------------------------------------
build/%.o: source/nonsecure/%.c | build
	@echo ==========================================================================================
	@echo                            Compiling $<
	@echo ==========================================================================================
	$(CC) $(CFLAGS) $(INCLUDES_NS) -c $< -o $@

# ------------------------------------------------------------
# Assemble startup
# ------------------------------------------------------------
build/%.o: source/nonsecure/%.s | build
	@echo ==========================================================================================
	@echo                            Assembling $<
	@echo ==========================================================================================
	$(AS) $(ASFLAGS) -c $< -o $@

# ------------------------------------------------------------
# Link ELF
# ------------------------------------------------------------
$(TARGET_NS).elf: $(OBJS_NS)
	@echo ==========================================================================================
	@echo                            Linking $(TARGET_NS).elf
	@echo ==========================================================================================
	$(LD) $(OBJS_NS) $(LDFLAGS_NS) -o $@
	$(SIZE) $@

build/%.o: source/secure/%.c | build
	@echo ==========================================================================================
	@echo                            Compiling $<
	@echo ==========================================================================================
	$(CC) $(CFLAGS) $(INCLUDES_S) -c $< -o $@
  
build/%.o: source/secure/%.s | build
	@echo ==========================================================================================
	@echo                            Assembling $<
	@echo ==========================================================================================
	$(AS) $(ASFLAGS) -c $< -o $@

# ------------------------------------------------------------
# Link ELF
# ------------------------------------------------------------
$(TARGET_S).elf: $(OBJS_S)
	@echo ==========================================================================================
	@echo                            Linking $(TARGET_S).elf
	@echo ==========================================================================================
	$(LD) $(OBJS_S) $(LDFLAGS) -o $@
	$(SIZE) $@

# ------------------------------------------------------------
# Clean
# ------------------------------------------------------------
clean:
	rm -rf build/

.PHONY: all clean bin