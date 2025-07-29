// cmd_mv.c
#include "core/shell.h"
#include "fs/ramfs.h"
#include <stddef.h>
#include <string.h>

// Move file command: mv <src> <dst>
// If <dst> is a directory, move <src> file inside <dst> directory preserving filename.
// If <dst> is a file or does not exist, rename <src> to <dst>.

void cmd_mv(const char* args) {
    // Skip leading spaces
    while (*args == ' ') args++;

    // Parse source argument
    const char* src = args;
    while (*args && *args != ' ') args++;
    int src_len = args - src;

    if (src_len == 0) {
        terminal_write("error: missing source file.\n");
        return;
    }

    // Skip spaces between src and dst
    while (*args == ' ') args++;

    // Parse destination argument
    const char* dst = args;
    while (*args && *args != ' ') args++;
    int dst_len = args - dst;

    if (dst_len == 0) {
        terminal_write("error: missing destination.\n");
        return;
    }

    // Copy src and dst names into buffers
    char src_name[MAX_FILENAME];
    char dst_name[MAX_FILENAME];

    if (src_len >= MAX_FILENAME || dst_len >= MAX_FILENAME) {
        terminal_write("error: file or directory name too long.\n");
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
        terminal_write("error: cannot move directory.\n");
        return;
    }

    // Try to find destination file or directory
    RAMFile* dst_file = ramfs_find_in_dir(current_dir, dst_name);
    RAMFile* dst_dir = ramfs_find_in_dir(current_dir, dst_name);

    // Check if destination is directory
    if (dst_file && dst_file->is_dir) {
        dst_dir = dst_file;  // Use as destination directory
        dst_file = NULL;     // Not a file overwrite
    } else if (dst_dir && !dst_dir->is_dir) {
        dst_dir = NULL;      // Not a directory
    } else if (!dst_file) {
        dst_dir = NULL;      // Not found, no directory
    }

    if (dst_dir) {
        // Moving file into directory dst_dir, keep original filename
        // Check if a file with the same name exists in dst_dir
        RAMFile* existing = ramfs_find_in_dir(dst_dir, src_name);
        if (existing) {
            ramfs_delete_file(existing);
        }

        // Remove source file from current directory
        ramfs_delete_file(src_file);

        // Create new file in destination directory with original name and content
        RAMFile* new_file = ramfs_create_file(dst_dir, src_name);
        if (!new_file) {
            terminal_write("error: cannot create file in destination directory.\n");
            return;
        }
        ramfs_write(new_file, src_file->data);

        terminal_write("File moved into directory.\n");
        return;
    }

    // Destination is a file or does not exist
    if (dst_file) {
        if (dst_file->is_dir) {
            terminal_write("error: destination is a directory.\n");
            return;
        }
        // Delete existing destination file
        ramfs_delete_file(dst_file);
    }

    // Rename source file directly by changing its name
    for (int i = 0; i < MAX_FILENAME; i++)
        src_file->name[i] = 0;
    for (int i = 0; i < dst_len; i++)
        src_file->name[i] = dst_name[i];

    terminal_write("File moved/renamed.\n");
}
