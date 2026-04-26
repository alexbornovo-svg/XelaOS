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

# Recursive wildcard to find all .c files in subdirectories
C_SOURCES := $(shell find $(SRC_DIR) -name "*.c")
# Map src/path/file.c to build/path/file.o
C_OBJS    := $(C_SOURCES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
# Assembly objects
BOOT_OBJ  := $(BUILD_DIR)/boot/loader.o

OBJS      := $(BOOT_OBJ) $(C_OBJS)

.PHONY: all run disk clean all_disk

all: $(ISO)

# Build the ISO
$(ISO): $(TARGET)
	@mkdir -p $(ISO_DIR)/grub
	cp $(TARGET) $(ISO_DIR)/kernel.elf
	# Ensure grub.cfg exists in iso/boot/grub/ before running this
	grub-mkrescue -o $@ iso/

# Link the Kernel
$(TARGET): $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS)

# Compile C Files
# Note: Added -MMD to generate .d dependency files automatically
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -MMD -c $< -o $@

# Assemble Bootloader
$(BOOT_OBJ): $(SRC_DIR)/boot/loader.s
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) $< -o $@

# Include the generated dependency files (.d)
-include $(OBJS:.o=.d)

# Variabili per comodità
DISK = $(BUILD_DIR)/disk.img
DISK_SIZE_MB = 32

disk:
	@mkdir -p $(BUILD_DIR)
	@echo "Creazione dell'immagine disco da $(DISK_SIZE_MB)MB..."
	dd if=/dev/zero of=$(DISK) bs=1M count=$(DISK_SIZE_MB)
	@echo "Formattazione in ext2..."
	# La flag -F forza la creazione del filesystem anche su un file regolare
	mkfs.ext2 -F $(DISK)

run: $(ISO) $(DISK)
	qemu-system-i386 \
		-drive file=$(ISO),format=raw,index=0,media=cdrom \
		-drive file=$(DISK),format=raw,index=1,media=disk \
		-boot d

clean:
	rm -rf $(BUILD_DIR)

all_disk: clean disk all run