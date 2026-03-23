ARCH := i686
TARGET := build/velocityos.elf
ISO := build/velocityos.iso
ISO_ROOT := build/isofiles

CC := i686-elf-gcc
LD := i686-elf-gcc
AS := nasm
GRUB_MKRESCUE := grub-mkrescue
QEMU := qemu-system-i386

CFLAGS := -std=gnu11 -ffreestanding -O2 -Wall -Wextra -Wshadow -Werror \
	-fno-stack-protector -fno-pic -m32 -Iinclude
LDFLAGS := -T linker.ld -ffreestanding -O2 -nostdlib -m32

KERNEL_OBJS := \
	build/boot.o \
	build/kernel.o

.PHONY: all clean iso run debug

all: $(ISO)

build:
	mkdir -p build

build/boot.o: src/boot.asm | build
	$(AS) -f elf32 $< -o $@

build/kernel.o: src/kernel.c | build
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(KERNEL_OBJS) linker.ld
	$(LD) $(LDFLAGS) $(KERNEL_OBJS) -o $@

$(ISO): $(TARGET) grub/grub.cfg
	mkdir -p $(ISO_ROOT)/boot/grub
	cp $(TARGET) $(ISO_ROOT)/boot/velocityos.elf
	cp grub/grub.cfg $(ISO_ROOT)/boot/grub/grub.cfg
	$(GRUB_MKRESCUE) -o $@ $(ISO_ROOT)

iso: $(ISO)

run: $(ISO)
	$(QEMU) -cdrom $(ISO) -m 128M

debug: $(ISO)
	$(QEMU) -cdrom $(ISO) -m 128M -s -S

clean:
	rm -rf build
