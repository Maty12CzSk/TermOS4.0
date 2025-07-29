#include "core/commands.h"
#include "fs/ramfs.h"
#include "core/io.h"
#include "core/shell.h"

extern RAMFile* current_dir;

void cmd_rm(const char* args) {
    RAMFile* f = ramfs_find_in_dir(current_dir, args);
    if (!f) {
        terminal_write("rm: no such file or directory\n");
        return;
    }
    ramfs_delete_file(f);
}
