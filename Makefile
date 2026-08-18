CC = gcc
LD = ld
CFLAGS = -m32 -O0 -fno-pie -fno-pic -ffreestanding -nostdlib -Ikernel -Ikernel/drivers/keyboard -Ikernel/commands
LDFLAGS = -m elf_i386 -T linker.ld -nostdlib

COMMANDS_SRC = $(wildcard kernel/commands/*.c)
COMMANDS_OBJ = $(COMMANDS_SRC:.c=.o)

all: rosos.iso

kernel/kernel.o: kernel/kernel.c kernel/drivers/keyboard/keyboard.h kernel/commands/commands.h
	$(CC) $(CFLAGS) -c $< -o $@

kernel/drivers/keyboard/keyboard.o: kernel/drivers/keyboard/keyboard.c kernel/drivers/keyboard/keyboard.h
	$(CC) $(CFLAGS) -c $< -o $@

$(COMMANDS_OBJ): kernel/commands/%.o: kernel/commands/%.c kernel/commands/commands.h
	$(CC) $(CFLAGS) -c $< -o $@

kernel.elf: kernel/kernel.o kernel/drivers/keyboard/keyboard.o $(COMMANDS_OBJ)
	$(LD) $(LDFLAGS) -o $@ $^

rosos.iso: kernel.elf grub.cfg
	mkdir -p iso/boot/grub
	cp kernel.elf iso/boot/
	cp grub.cfg iso/boot/grub/
	grub-mkrescue -o rosos.iso iso
	rm -rf iso

clean:
	rm -f *.o *.elf *.iso kernel/kernel.o kernel/drivers/keyboard/keyboard.o $(COMMANDS_OBJ)
	rm -rf iso
