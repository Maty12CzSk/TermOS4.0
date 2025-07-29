#include "core/commands.h"
#include "fs/ramfs.h"
#include "core/io.h"
#include "core/shell.h"

extern RAMFile* current_dir;

void truncate_command(const char* args) {
    if (!args || *args == 0) {
        terminal_write("truncate: missing filename\n");
        return;
    }
    RAMFile* f = ramfs_find_in_dir(current_dir, args);
    if (!f) {
        terminal_write("truncate: file not found\n");
        return;
    }
    if (f->is_dir) {
        terminal_write("truncate: cannot truncate a directory\n");
        return;
    }
    f->size = 0;
    f->data[0] = 0;
    terminal_write("File truncated.\n");
}
