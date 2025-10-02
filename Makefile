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

# --------------------------------------------------------------------
# Règles spéciales
# --------------------------------------------------------------------
.PHONY: all build run run-elf clean_obj clean_iso clean_elf clean_obj_iso fclean re
.PRECIOUS: $(OBJDIR) $(OBJDIR)/

# --------------------------------------------------------------------
# Règle par défaut : affiche l'aide
# --------------------------------------------------------------------
all:
	@echo "Available make targets:"
	@echo "  build         -> compile et génère $(TARGET) et $(ISO)"
	@echo "  run           -> exécute $(ISO) avec QEMU"
	@echo "  run-elf       -> exécute $(TARGET) directement avec QEMU"
	@echo "  clean_obj     -> supprime les objets compilés ($(OBJDIR))"
	@echo "  clean_iso     -> supprime l'iso et le dossier iso/"
	@echo "  clean_elf     -> supprime le kernel.elf"
	@echo "  clean_obj_iso -> supprime les objets compilés, l'iso et le dossier iso"
	@echo "  fclean        -> clean_obj + clean_iso + clean_elf"
	@echo "  re            -> fclean puis build"

# --------------------------------------------------------------------
# Compilation des objets
# --------------------------------------------------------------------
$(OBJDIR)/boot.o: boot.s
	mkdir -p $(dir $@)
	$(ASM) $(ASMFLAGS) $< -o $@

$(OBJDIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# --------------------------------------------------------------------
# Kernel + ISO
# --------------------------------------------------------------------
$(TARGET): $(OBJDIR)/boot.o $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^

$(ISO): $(TARGET)
	mkdir -p iso/boot/grub
	cp $(TARGET) iso/boot/
	cp grub/grub.cfg iso/boot/grub/
	grub-mkrescue -o $(ISO) iso --modules="normal multiboot iso9660" \
		--fonts="" --locales="" --themes="" \
		--compress=xz 2>/dev/null

# --------------------------------------------------------------------
# Helpers
# --------------------------------------------------------------------
build: $(ISO)

run: $(ISO)
	qemu-system-i386 -cdrom $(ISO)

run-elf: $(TARGET)
	qemu-system-i386 -kernel $(TARGET)

# --------------------------------------------------------------------
# Clean rules
# --------------------------------------------------------------------
clean_obj:
	rm -rf $(OBJDIR)

clean_iso:
	rm -rf iso $(ISO)

clean_elf:
	rm -f $(TARGET)

clean_obj_iso: clean_obj clean_iso

fclean: clean_obj clean_iso clean_elf

re: fclean build
