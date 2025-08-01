CC = gcc
LD = ld
CFLAGS = -m32 -ffreestanding -O2 -Wall -Wextra
LDFLAGS = -m elf_i386 -T linker.ld

OBJS = boot.o kernel.o ramfs.o keymapping.o commands.o shell.o utils.o colors.o history.o nano.o

all: kernel.iso


nano.o: nano.c
	$(CC) $(CFLAGS) -c nano.c -o nano.o

history.o: history.c
	$(CC) $(CFLAGS) -c history.c -o history.o

colors.o: colors.c
	$(CC) $(CFLAGS) -c colors.c -o colors.o

utils.o: utils.c
	$(CC) $(CFLAGS) -c utils.c -o utils.o

commands.o: commands.c
	$(CC) $(CFLAGS) -c commands.c -o commands.o

shell.o: shell.c
	$(CC) $(CFLAGS) -c shell.c -o shell.o

keymapping.o: keymapping.c
	$(CC) $(CFLAGS) -c keymapping.c -o keymapping.o

ramfs.o: ramfs.c
	$(CC) $(CFLAGS) -c ramfs.c -o ramfs.o

boot.o: boot.s
	nasm -f elf32 boot.s -o boot.o

kernel.o: kernel.c
	$(CC) $(CFLAGS) -c kernel.c -o kernel.o

kernel.bin: $(OBJS) linker.ld
	$(LD) $(LDFLAGS) -o kernel.bin $(OBJS)

isofiles/boot/kernel.bin: kernel.bin
	mkdir -p isofiles/boot
	cp kernel.bin isofiles/boot/kernel.bin

isofiles/boot/grub/grub.cfg:
	mkdir -p isofiles/boot/grub
	echo 'set timeout=5' > isofiles/boot/grub/grub.cfg
	echo 'set default=0' >> isofiles/boot/grub/grub.cfg
	echo 'menuentry "TermOS 4.0" {' >> isofiles/boot/grub/grub.cfg
	echo '    multiboot /boot/kernel.bin' >> isofiles/boot/grub/grub.cfg
	echo '    boot' >> isofiles/boot/grub/grub.cfg
	echo '}' >> isofiles/boot/grub/grub.cfg
	echo '' >> isofiles/boot/grub/grub.cfg
	echo 'menuentry "DummyOS (Example, does nothing)" {' >> isofiles/boot/grub/grub.cfg
	echo '    echo "No OS loaded. (DummyOS example entry)"' >> isofiles/boot/grub/grub.cfg
	echo '    sleep 2' >> isofiles/boot/grub/grub.cfg
	echo '}' >> isofiles/boot/grub/grub.cfg

kernel.iso: isofiles/boot/kernel.bin isofiles/boot/grub/grub.cfg
	grub-mkrescue -o kernel.iso isofiles

clean:
	rm -f *.o kernel.bin kernel.iso
	rm -rf isofiles

run: kernel.iso
	qemu-system-i386 -cdrom kernel.iso

.PHONY: all clean run