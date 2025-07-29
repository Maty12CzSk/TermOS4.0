// src/cmd/cmd_ls.c
#include "core/shell.h"
#include "fs/ramfs.h"
#include "core/shell.h"

extern RAMFile* current_dir;

void cmd_ls(void) {
    int found = 0;
    for (int i = 0; i < MAX_FILES; ++i) {
        RAMFile* file = ramfs_file_at(i);
        if (file->used && file->parent == current_dir) {
            terminal_write(file->name);
            terminal_putchar('\n');
            found = 1;
        }
    }

    if (!found) {
        terminal_write("No files.\n");
    }
}
