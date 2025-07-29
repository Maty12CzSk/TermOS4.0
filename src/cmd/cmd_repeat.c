// cmd_repeat.c
#include "core/history.h"
#include "core/shell.h"
#include "core/commands.h"
#include <stddef.h>


static int parse_number(const char* s) {
    int num = 0;
    if (!s || *s == 0) return -1;
    for (int i = 0; s[i] != 0; i++) {
        if (s[i] < '0' || s[i] > '9') return -1;
        num = num * 10 + (s[i] - '0');
        if (num > 999) return -1;
    }
    return num;
}

// cmd_repeat: execute command number n from history
void cmd_repeat(const char* args) {
    // args expected to be "n" (e.g. "!3" → args="3")
    int n = parse_number(args);
    if (n <= 0) {
        terminal_write("error: invalid command number.\n");
        return;
    }

    int count = history_size();
    if (n > count) {
        terminal_write("error: command number out of range.\n");
        return;
    }

    const char* cmd = history_get(n - 1);
    if (!cmd) {
        terminal_write("error: command not found.\n");
        return;
    }

    terminal_write(cmd);
    terminal_write("\n");


    handle_command(cmd);
}
