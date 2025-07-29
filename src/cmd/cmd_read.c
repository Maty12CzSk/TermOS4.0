#include "core/commands.h"
#include "fs/ramfs.h"
#include "core/io.h"
#include "core/utils.h"
#include <stddef.h>
#include "core/shell.h"

void cmd_read(const char* cmd) {
    // cmd format: "read [option] filename"
    const char* args = cmd + 4; 
    while (*args == ' ') args++;

    int mode_hex = 0;  // 1 = hex output, 0 = line output

    if (*args == '-') {
        
        if (args[1] == 'h') {
            mode_hex = 1;
        } else if (args[1] == 'l') {
            mode_hex = 0;
        } else {
            terminal_write("Usage: read [-h|-l] <filename>\n");
            return;
        }
        args += 2;
        while (*args == ' ') args++;
    }

    if (*args == '\0') {
        terminal_write("read: missing filename\n");
        return;
    }

    RAMFile* f = ramfs_find_in_dir(current_dir, args);
    if (!f) {
        terminal_write("read: file not found\n");
        return;
    }
    if (f->is_dir) {
        terminal_write("read: cannot read a directory\n");
        return;
    }

    if (mode_hex) {
        // Hex output
        for (size_t i = 0; i < f->size; ++i) {
            char hex[3];
            int_to_hex_str((unsigned char)f->data[i], hex);
            terminal_write(hex);
            terminal_write(" ");
            if ((i + 1) % 16 == 0) terminal_putchar('\n');
        }
        if (f->size % 16 != 0) terminal_putchar('\n');
    } else {
        // Line output (text)
        for (size_t i = 0; i < f->size; ++i) {
            char c = f->data[i];
            terminal_putchar(c);
            if (c == '\n' || c == '\r') {
            }
        }
        // if last symbol isnt new line, add
        if (f->size > 0 && f->data[f->size - 1] != '\n')
            terminal_putchar('\n');
    }
}
