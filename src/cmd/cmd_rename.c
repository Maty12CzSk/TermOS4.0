#include "core/commands.h"
#include "fs/ramfs.h"
#include "core/io.h"
#include <string.h>
#include "core/shell.h"

extern RAMFile* current_dir;

void cmd_rename(const char* args) {
    const char* space = strchr(args, ' ');
    if (!space) {
        terminal_write("usage: rename <src> <name>\n");
        return;
    }

    int src_len = space - args;
    if (src_len <= 0 || src_len >= MAX_FILENAME) {
        terminal_write("rename: invalid source name\n");
        return;
    }

    char src_name[MAX_FILENAME];
    strncpy(src_name, args, src_len);
    src_name[src_len] = 0;

    const char* dst_name = space + 1;
    if (strlen(dst_name) == 0 || strlen(dst_name) >= MAX_FILENAME) {
        terminal_write("rename: max filename length is 16\n");
        return;
    }

    RAMFile* src_file = ramfs_find_in_dir(current_dir, src_name);
    if (!src_file) {
        terminal_write("rename: source not found\n");
        return;
    }

    RAMFile* dst_file = ramfs_find_in_dir(current_dir, dst_name);
    if (dst_file) {
        terminal_write("rename: file already exists\n");
        return;
    }

    strncpy(src_file->name, dst_name, MAX_FILENAME - 1);
    src_file->name[MAX_FILENAME - 1] = 0;

    terminal_write("rename: File renamed.\n");
}
