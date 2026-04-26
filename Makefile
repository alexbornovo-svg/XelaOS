AS      = nasm
CC      = gcc
LD      = ld

# Flags
ASFLAGS = -f elf32
CFLAGS  = -m32 -ffreestanding -fno-builtin -nostdlib -fno-stack-protector -Wall -Wextra -Isrc/include
LDFLAGS = -m elf_i386 -T linker.ld

# Directories
BUILD_DIR = build
ISO_DIR   = iso/boot
SRC_DIR   = src

# Files
TARGET    = $(BUILD_DIR)/kernel.elf
ISO       = $(BUILD_DIR)/XelaOS.iso
DISK      = $(BUILD_DIR)/disk.img

# Objects
BOOT_OBJ  := $(BUILD_DIR)/boot/loader.o
ASM_OBJS  := $(BUILD_DIR)/misc/isr_asm.o
C_SOURCES := $(shell find $(SRC_DIR) -name "*.c")
C_OBJS    := $(C_SOURCES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
OBJS      := $(BOOT_OBJ) $(ASM_OBJS) $(C_OBJS)

DISK_SIZE_MB = 32

.PHONY: all run disk clean all_disk

all: $(ISO)

# Build the ISO
$(ISO): $(TARGET)
	@mkdir -p $(ISO_DIR)/grub
	cp $(TARGET) $(ISO_DIR)/kernel.elf
	grub-mkrescue -o $@ iso/

# Link the Kernel
$(TARGET): $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS)

# Compile C Files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -MMD -c $< -o $@

# Assemble Bootloader
$(BOOT_OBJ): $(SRC_DIR)/boot/loader.s
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) $< -o $@

# Assemble ISR stubs
$(BUILD_DIR)/misc/isr_asm.o: $(SRC_DIR)/misc/isr.s
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) $< -o $@

# Include dependency files
-include $(OBJS:.o=.d)

disk:
	@mkdir -p $(BUILD_DIR)
	@echo "Creazione dell'immagine disco da $(DISK_SIZE_MB)MB..."
	dd if=/dev/zero of=$(DISK) bs=1M count=$(DISK_SIZE_MB)
	@echo "Formattazione in ext2..."
	mkfs.ext2 -F $(DISK)

run: $(ISO) $(DISK)
	qemu-system-i386 \
		-drive file=build/XelaOS.iso,format=raw,index=0,media=cdrom \
		-drive file=build/disk.img,format=raw,index=1,media=disk \
		-boot d \
		-display gtk \
		-s -S \
		-M accel=tcg,smm=off -d int -no-reboot -no-shutdown -D qemu_log.txt

clean:
	rm -rf $(BUILD_DIR)

all_disk: clean disk all run