#include "core/shell.h"
#include "fs/ramfs.h"
#include "core/utils.h"

static void append_str(char* dest, size_t* pos, const char* src) {
    size_t len = strlen(src);
    memcpy(dest + *pos, src, len);
    *pos += len;
    dest[*pos] = '\0';
}

void cmd_sysinfo() {
    char buf[256] = {0};
    size_t pos = 0;
    char temp[16];

    int file_count = 0;
    int dir_count = 0;
    size_t total_size = 0;

    for (int i = 0; i < MAX_FILES; i++) {
        RAMFile* f = &root_dir[i];
        if (f->used) {
            if (f->is_dir) {
                dir_count++;
            } else {
                file_count++;
                total_size += f->size;
            }
        }
    }

    append_str(buf, &pos, "TermOS System Info\n");
    append_str(buf, &pos, "OS: TermOS v4.1\n");
    append_str(buf, &pos, "Arch: i386 (x86)\n");
    append_str(buf, &pos, "------------------\nFiles: ");
    int_to_str(file_count, temp);
    append_str(buf, &pos, temp);

    append_str(buf, &pos, "\nDirs: ");
    int_to_str(dir_count, temp);
    append_str(buf, &pos, temp);

    append_str(buf, &pos, "\nUsed RAMFS size: ");
    int_to_str((int)total_size, temp);
    append_str(buf, &pos, temp);

    append_str(buf, &pos, " bytes\n");

    terminal_write(buf);
}
