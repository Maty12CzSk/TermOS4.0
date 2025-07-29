#include "core/commands.h"
#include "fs/ramfs.h"
#include "core/shell.h"
#include <string.h>

void cmd_cd(const char *name) {
    while (*name == ' ') ++name;
    if (strlen(name) == 0) {
        terminal_write("cd: missing directory\n");
        return;
    }

    if (!strcmp(name, "..")) {
        if (current_dir->parent) {
            current_dir = current_dir->parent;
        } else {
            terminal_write("Already at root.\n");
        }
        return;
    }

    RAMFile* d = ramfs_find_in_dir(current_dir, name);
    if (!d) {
        terminal_write("cd: no such directory\n");
    } else if (!d->is_dir) {
        terminal_write("cd: not a directory\n");
    } else {
        current_dir = d;
        terminal_write("Directory changed.\n");
    }
}
