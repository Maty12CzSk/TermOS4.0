#include "core/commands.h"
#include "fs/ramfs.h"
#include "core/io.h"
#include "core/shell.h"
#include <string.h>

extern RAMFile* current_dir;

void cmd_touch(const char* args) {
    if (strlen(args) == 0) {
        terminal_write("touch: missing filename\n");
        return;
    }
    RAMFile* f = ramfs_create_file(current_dir, args);
    if (!f) {
        terminal_write("touch: cannot create file\n");
    }
}
