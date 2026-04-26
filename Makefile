AS      = nasm
CC      = gcc
ASFLAGS = -f elf32
CFLAGS  = -m32 -ffreestanding -fno-builtin -nostdlib -fno-stack-protector -Isrc/include
LDFLAGS = -m elf_i386 -T linker.ld
BUILD_DIR = build
ISO_DIR   = iso/boot
TARGET    = $(BUILD_DIR)/kernel.elf
ISO       = $(BUILD_DIR)/XelaOS.iso
C_SOURCES := $(wildcard src/kernel/*.c) \
             $(wildcard src/drivers/*.c) \
             $(wildcard src/libraries/*.c) \
             $(wildcard src/misc/*.c)
H_INCLUDE := $(wildcard src/include/*.h)
C_OBJS := $(patsubst src/%.c, $(BUILD_DIR)/%.o, $(C_SOURCES))
OBJS = $(BUILD_DIR)/boot.o $(C_OBJS)
all: $(ISO)
$(BUILD_DIR)/boot.o: src/boot/loader.s
	@mkdir -p $(BUILD_DIR)
	$(AS) $(ASFLAGS) $< -o $@
$(BUILD_DIR)/%.o: src/%.c $(H_INCLUDE)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@
$(TARGET): $(OBJS)
	ld $(LDFLAGS) -o $@ $(OBJS)
$(ISO): $(TARGET)
	@mkdir -p $(ISO_DIR)/grub
	cp $(TARGET) $(ISO_DIR)/kernel.elf
	grub-mkrescue -o $@ iso/
run: $(ISO)
	qemu-system-i386 -cdrom $(ISO)
clean:
	rm -rf $(BUILD_DIR)/
.PHONY: all run clean