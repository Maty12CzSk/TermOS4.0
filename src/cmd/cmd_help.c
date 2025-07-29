#include "core/commands.h"
#include "core/colors.h"
#include "core/shell.h"
#include <string.h>

void terminal_write_padded(const char* str, int width) {
    int len = strlen(str);
    terminal_write(str);
    for (int i = len; i < width; i++) {
        terminal_putchar(' ');
    }
}

void cmd_help() {
    const char* commands[] = {
        "help - show help",
        "clear - clear screen",
        "about - about OS",
        "ls - list files",
        "cat <file> - show file",
        "touch <file> - create file",
        "mkdir <dir> - make dir",
        "rm <file|dir> - remove",
        "cd <dir> - change dir",
        "pwd - current dir path",
        "echo <txt>",
        "alias [name=cmd]",
        "unalias <name>",
        "mv <src> <dst>",
        "cp <src> <dst>",
        "cmp <f1> <f2> - compare",
        "rename <src> <dst>",
        "truncate <file>",
        "stat <file> - file info",
        "read -h (hex)/ -l (line)",
        "source/run <file>",
        "color <0-15>",
        "reset - reset color",
        "banner - show banner",
        "nano <file> - editor",
        "rand - random number",
        "rainbow <text>",
        "time - current time",
        "date - current date",
        "uptime - system ticks",
        "whoami - print user",
        "history - cmd history",
        "!<n> - repeat history",
        "beep - beep speaker",
        "shutdown - halt CPU",
        "reboot - restart"
    };
    int count = sizeof(commands)/sizeof(commands[0]);
    int cols = 3;
    int width = 25;
    set_fg_color(0x03);

    for (int i = 0; i < count; i++) {
        terminal_write_padded(commands[i], width);
        if ((i+1) % cols == 0) {
            terminal_putchar('\n');
        }
    }
    if (count % cols != 0) {
        terminal_putchar('\n');
    }
    reset_color();
}

