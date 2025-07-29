#include "core/commands.h"
#include "fs/ramfs.h"
#include "core/io.h"
#include "core/shell.h"
#include <string.h>

extern RAMFile* current_dir;

void cmd_mkdir(const char* args) {
    if (strlen(args) == 0) {
        terminal_write("mkdir: missing directory name\n");
        return;
    }
    RAMFile* d = ramfs_create_dir(current_dir, args);
    if (!d) {
        terminal_write("mkdir: directory already exists or cannot create directory\n");
    }
}
