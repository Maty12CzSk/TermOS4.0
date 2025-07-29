#include "core/commands.h"
#include "fs/ramfs.h"
#include "core/shell.h"
#include "core/utils.h"

void run_script(const char *filename) {
    static int recursion_depth = 0;
    if (recursion_depth > 10) {
        terminal_write("Maximum script depth reached.\n");
        return;
    }

    recursion_depth++;

    RAMFile* f = ramfs_find_in_dir(current_dir, filename);
    if (!f) {
        terminal_write("No such file.\n");
        recursion_depth--;
        return;
    }
    if (f->is_dir) {
        terminal_write("Cannot run a directory.\n");
        recursion_depth--;
        return;
    }

    char buf[1024] = {0};
    size_t len = f->size;
    if (len > sizeof(buf) - 1) len = sizeof(buf) - 1;
    memcpy(buf, f->data, len);
    buf[len] = '\0';

    char *line = buf;
    while (*line) {
        while (*line == '\r' || *line == '\n') ++line;
        if (!*line) break;

        char *next = line;
        while (*next && *next != '\n' && *next != '\r') ++next;

        char old = *next;
        *next = '\0';

        char *end = next - 1;
        while (end >= line && (*end == ' ' || *end == '\t' || *end == '\r' || *end == '\n')) {
            *end = '\0';
            end--;
        }

        if (line[0]) {
            terminal_putchar('\n');

            run_commands_separated(line);
        }

        *next = old;
        if (*next) next++;
        line = next;
    }

    recursion_depth--;
}