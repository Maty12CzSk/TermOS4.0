# TermOS 4.1

TermOS 4.1 is a fully functional minimalist operating system developed as a personal project. It is written in C and Assembly, focusing on core OS concepts and functionality.

## Features

- Basic bootloader implemented in assembly (`boot.s`)
- Text-mode terminal and simple shell with basic commands (`shell.c`, `commands.c`)
- Keyboard input handling (`keymapping.c`)
- Simple text editor inspired by nano (`nano.c`)
- RAM-based file system (RAMFS) for fast and simple file operations (`ramfs.c`)
- Command history with navigation and command repetition (`history.c`)
- Alias system to create and manage command shortcuts (`alias.c`)
- Basic color schemes and terminal output formatting
- Support for script running and command chaining (separated by `;`)
- Common shell commands implemented: `ls`, `cd`, `cp`, `mv`, `rm`, `mkdir`, `cat`, `touch`, `alias`, `unalias`, and more

## Current Limitations

- FAT16 file system support is under development and not included in this stable version (it will be included in TermOS v5.0)
- No graphical user interface yet; purely text-mode OS

## Getting Started

To build and run TermOS 4.1:

1. Clone the repository  
2. Use `make` to compile the OS  
3. Run it easily with `make run`, which will automatically launch the OS in QEMU

```bash
make
make run
