# TermOS 4.0

TermOS 4.0 is a fully functional minimalist operating system developed as a personal project.  
It is written in C and Assembly, focusing on core OS concepts and terminal-based functionality.

## Features

- Basic bootloader implemented in assembly (`boot.s`)
- Text-mode terminal and simple shell with basic commands (`shell.c`, `commands.c`)
- Keyboard input handling (`keymapping.c`)
- Simple text editor inspired by nano (`nano.c`)
- RAM-based file system (RAMFS) for fast and simple file operations (`ramfs.c`)
- Command history and utility functions
- Basic color schemes and terminal output formatting

## Current Limitations

- FAT16 file system support is under development and not yet functional in this version
- No graphical user interface; purely text-mode OS

## Getting Started

To build and run TermOS 4.0:

1. Clone the repository  
2. Use `make` to compile the OS  
3. Run it easily with `make run`, which will automatically launch the OS in QEMU

```bash
make
make run

## Fedora Compatibility Note

If you're using Fedora, you may need to install some packages and modify the Makefile:

sudo dnf install glibc-devel.i686 xorriso

And change grub-mkconfig to grub2-mkconfig in the makefile if needed.