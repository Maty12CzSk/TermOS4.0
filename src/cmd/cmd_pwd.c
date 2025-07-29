#include "core/commands.h"
#include "fs/ramfs.h"
#include "core/shell.h"

extern RAMFile* current_dir;


void cmd_pwd(void) {
    RAMFile* path_stack[MAX_PATH_DEPTH];
    int depth = 0;
    RAMFile* dir = current_dir;

    while (dir != NULL && dir != root_dir && depth < MAX_PATH_DEPTH) {
        path_stack[depth++] = dir;
        dir = dir->parent;
    }

    terminal_write("~");
    for (int i = depth - 1; i >= 0; i--) {
        terminal_write("/");
        terminal_write(path_stack[i]->name);
    }
    terminal_putchar('\n');
}
