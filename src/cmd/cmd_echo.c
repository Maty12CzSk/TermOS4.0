#include "core/commands.h"
#include "fs/ramfs.h"
#include "core/io.h"
#include <string.h>
#include "core/shell.h"

extern RAMFile* current_dir;

void cmd_echo(const char* args) {
    const char *p = args;
    const char *redir = strstr(p, " > ");
    const char *append = strstr(p, " >> ");
    if (redir || append) {
        size_t textlen = (redir ? redir : append) - p;
        char text[128] = {0};
        if (textlen > 127) textlen = 127;
        memcpy(text, p, textlen);
        text[textlen] = 0;

        const char *fname = (redir ? redir + 3 : append + 4);
        while (*fname == ' ') fname++;
        if (strlen(fname) == 0) {
            terminal_write("echo: missing filename after >\n");
            return;
        }
        RAMFile* f = ramfs_find_in_dir(current_dir, fname);
        if (!f) {
            f = ramfs_create_file(current_dir, fname);
            if (!f) {
                terminal_write("echo: cannot create file\n");
                return;
            }
        }
        if (append)
            ramfs_append(f, text);
        else
            ramfs_write(f, text);
    } else {
        terminal_write(p);
        terminal_putchar('\n');
    }
}
