#include "core/commands.h"
#include "fs/ramfs.h"
#include "core/io.h"
#include "core/utils.h"
#include "core/shell.h"

extern RAMFile* current_dir;

void cmd_stat(const char* filename) {
    RAMFile* f = ramfs_find_in_dir(current_dir, filename);
    if (!f) {
        terminal_write("File not found.\n");
        return;
    }
    terminal_write("Filename: ");
    terminal_write(filename);
    terminal_write("\n");

    if (f->is_dir) {
        terminal_write("Type: Directory\n");
    } else {
        terminal_write("Type: File\n");
        char size_str[32];
        int_to_str(f->size, size_str);
        terminal_write("Size: ");
        terminal_write(size_str);
        terminal_write(" bytes\n");
    }
}
