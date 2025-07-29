#include "core/commands.h"
#include "fs/ramfs.h"
#include "core/io.h"
#include "core/shell.h"

extern RAMFile* current_dir;

void cmd_cat(const char* args) {
    RAMFile* f = ramfs_find_in_dir(current_dir, args);
    if (!f) {
        terminal_write("No such file.\n");
        return;
    }
    if (f->is_dir) {
        terminal_write("cat: cannot cat a directory\n");
        return;
    }
    terminal_write(f->data);
    if (f->size > 0 && f->data[f->size - 1] != '\n')
        terminal_putchar('\n');
}
