TARGET = kernel.elf
ISO = kernel.iso

CC = gcc
CFLAGS = -m32 -ffreestanding -O2 -Wall -Wextra -nostdlib -nodefaultlibs -fno-stack-protector -fno-pie -Iinclude
LD = ld
LDFLAGS = -m elf_i386 -T linker.ld
ASM = nasm
ASMFLAGS = -f elf32

OBJDIR = build

SRCS = kernel.c $(wildcard src/*.c)
OBJS = $(patsubst %.c,$(OBJDIR)/%.o,$(SRCS))

all: $(ISO)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)/boot.o: boot.s | $(OBJDIR)
	$(ASM) $(ASMFLAGS) $< -o $@

$(OBJDIR)/%.o: %.c | $(OBJDIR)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJDIR)/boot.o $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^

$(ISO): $(TARGET)
	mkdir -p iso/boot/grub
	cp $(TARGET) iso/boot/
	cp grub/grub.cfg iso/boot/grub/
	grub-mkrescue -o $(ISO) iso

run: $(ISO)
	qemu-system-i386 -cdrom $(ISO)

run-elf: $(TARGET)
	qemu-system-i386 -kernel $(TARGET)

clean:
	rm -rf $(OBJDIR) iso $(ISO)

re: clean all
