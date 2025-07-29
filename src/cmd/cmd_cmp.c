#include "core/io.h"
#include "core/commands.h"
#include "fs/ramfs.h"
#include <string.h>

extern RAMFile* current_dir;

// rozdělení args na dvě části (file1 a file2)
int parse_two_filenames(const char* args, char* filename1, char* filename2) {
    // najdi první mezeru
    const char* space = strchr(args, ' ');
    if (!space) return 0;
    size_t len1 = space - args;
    if (len1 >= 64) len1 = 63;
    strncpy(filename1, args, len1);
    filename1[len1] = '\0';

    // zbytek je filename2, ořízni počáteční mezery
    const char* p = space + 1;
    while (*p == ' ') p++;
    strncpy(filename2, p, 63);
    filename2[63] = '\0';

    return 1;
}

void cmd_cmp(const char* args) {
    char filename1[64], filename2[64];
    if (!parse_two_filenames(args, filename1, filename2)) {
        terminal_write("cmp: usage: cmp <file1> <file2>\n");
        return;
    }

    int idx1 = ramfs_find(filename1);
    int idx2 = ramfs_find(filename2);

    if (idx1 == -1) {
        terminal_write("cmp: file1 not found\n");
        return;
    }
    if (idx2 == -1) {
        terminal_write("cmp: file2 not found\n");
        return;
    }   

    RAMFile* f1 = ramfs_file_at(idx1);
    RAMFile* f2 = ramfs_file_at(idx2);

    if (!f1 || !f2) {
        terminal_write("cmp: one or both files not found\n");
        return;
    }

    if (f1->size != f2->size) {
        terminal_write("cmp: files differ (size)\n");
        return;
    }

    for (size_t i = 0; i < f1->size; i++) {
        if (f1->data[i] != f2->data[i]) {
            terminal_write("cmp: files differ (content)\n");
            return;
        }
    }

    terminal_write("cmp: files are identical\n");
}
