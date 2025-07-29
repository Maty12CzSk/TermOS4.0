CC = gcc
LD = ld
CFLAGS = -m32 -ffreestanding -O2 -Wall -Wextra -mno-sse -mno-mmx -mno-3dnow -mno-avx -Iinclude
LDFLAGS = -m elf_i386 -T linker.ld

SRC = \
	src/core/kernel.c \
	src/core/keymapping.c \
	src/core/commands.c \
	src/core/shell.c \
	src/core/utils.c \
	src/core/alias.c \
	src/core/colors.c \
	src/core/history.c \
	src/core/run_script.c \
	src/fs/ramfs.c \
	src/cmd/cmd_about.c \
	src/cmd/cmd_banner.c \
	src/cmd/cmd_beep.c \
	src/cmd/cmd_cat.c \
	src/cmd/cmd_cd.c \
	src/cmd/cmd_clear.c \
	src/cmd/cmd_color.c \
	src/cmd/cmd_date.c \
	src/cmd/cmd_echo.c \
	src/cmd/cmd_help.c \
	src/cmd/cmd_ls.c \
	src/cmd/cmd_mkdir.c \
	src/cmd/cmd_pwd.c \
	src/cmd/cmd_reboot.c \
	src/cmd/cmd_rename.c \
	src/cmd/cmd_reset.c \
	src/cmd/cmd_rm.c \
	src/cmd/cmd_shutdown.c \
	src/cmd/cmd_stat.c \
	src/cmd/cmd_sysinfo.c \
	src/cmd/cmd_time.c \
	src/cmd/cmd_touch.c \
	src/cmd/cmd_truncate.c \
	src/cmd/cmd_uptime.c \
	src/cmd/cmd_whoami.c \
	src/cmd/cmd_random.c \
	src/cmd/cmd_read.c \
	src/cmd/cmd_cmp.c \
	src/cmd/cmd_cp.c \
	src/cmd/cmd_history.c \
	src/cmd/cmd_repeat.c \
	src/cmd/cmd_mv.c \
	src/cmd/cmd_alias.c \
	src/cmd/cmd_nano.c



OBJS = $(SRC:.c=.o) boot.o

all: kernel.iso

# compilate every *.c > *.o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Assembly
boot.o: boot.s
	nasm -f elf32 boot.s -o boot.o

# Linker
kernel.bin: $(OBJS) linker.ld
	$(LD) $(LDFLAGS) -o kernel.bin $(OBJS)

# ISO image
isofiles/boot/kernel.bin: kernel.bin
	mkdir -p isofiles/boot
	cp kernel.bin isofiles/boot/kernel.bin

isofiles/boot/grub/grub.cfg:
	mkdir -p isofiles/boot/grub
	echo 'set timeout=5' > isofiles/boot/grub/grub.cfg
	echo 'set default=0' >> isofiles/boot/grub/grub.cfg
	echo 'menuentry "TermOS 4.1" {' >> isofiles/boot/grub/grub.cfg
	echo '    multiboot /boot/kernel.bin' >> isofiles/boot/grub/grub.cfg
	echo '    boot' >> isofiles/boot/grub/grub.cfg
	echo '}' >> isofiles/boot/grub/grub.cfg

kernel.iso: isofiles/boot/kernel.bin isofiles/boot/grub/grub.cfg
	grub-mkrescue -o kernel.iso isofiles

clean:
	rm -f $(OBJS) boot.o kernel.bin kernel.iso
	rm -rf isofiles

run: kernel.iso
	qemu-system-i386 -cdrom kernel.iso -d int,cpu_reset -no-reboot

.PHONY: all clean run
