// cmd_cp.c
#include "core/shell.h"
#include "fs/ramfs.h"
#include <stddef.h>

void cmd_cp(const char* args) {
    // Parse arguments <src> <dst>
    while (*args == ' ') args++;

    const char* src = args;
    while (*args && *args != ' ') args++;
    int src_len = args - src;

    if (src_len == 0) {
        terminal_write("error: missing source file.\n");
        return;
    }

    while (*args == ' ') args++;

    const char* dst = args;
    while (*args && *args != ' ') args++;
    int dst_len = args - dst;

    if (dst_len == 0) {
        terminal_write("error: missing destination file.\n");
        return;
    }

    // Copy names into buffers
    char src_name[MAX_FILENAME];
    char dst_name[MAX_FILENAME];

    if (src_len >= MAX_FILENAME || dst_len >= MAX_FILENAME) {
        terminal_write("error: file name too long.\n");
        return;
    }

    for (int i = 0; i < src_len; i++) src_name[i] = src[i];
    src_name[src_len] = '\0';
    for (int i = 0; i < dst_len; i++) dst_name[i] = dst[i];
    dst_name[dst_len] = '\0';

    // Find source file in current directory
    RAMFile* src_file = ramfs_find_in_dir(current_dir, src_name);
    if (!src_file) {
        terminal_write("error: source file not found.\n");
        return;
    }
    if (src_file->is_dir) {
        terminal_write("error: cannot copy a directory.\n");
        return;
    }

    // Find destination file or create a new one
    RAMFile* dst_file = ramfs_find_in_dir(current_dir, dst_name);
    if (!dst_file) {
        dst_file = ramfs_create_file(current_dir, dst_name);
        if (!dst_file) {
            terminal_write("error: cannot create destination file (filesystem full?).\n");
            return;
        }
    } else {
        if (dst_file->is_dir) {
            terminal_write("error: destination file is a directory.\n");
            return;
        }
    }

    // Overwrite destination file data with source file data
    ramfs_write(dst_file, src_file->data);

    terminal_write("File copied successfully.\n");
}
