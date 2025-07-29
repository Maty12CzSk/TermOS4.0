// cmd_alias.c
#include "core/shell.h"
#include "core/alias.h"
#include <string.h>

void cmd_alias(const char* args) {
    // If no arguments, print all aliases
    if (!args || !*args) {
        for (int i = 0; i < MAX_ALIASES; i++) {
            if (aliases[i].used) {
                terminal_write("alias ");
                terminal_write(aliases[i].name);
                terminal_write("='");
                terminal_write(aliases[i].command);
                terminal_write("'\n");
            }
        }
        return;
    }

    // Expect args: "name=command"
    const char* equal_pos = strchr(args, '=');
    if (!equal_pos) {
        terminal_write("error: invalid alias format (expected name=command)\n");
        return;
    }

    int name_len = equal_pos - args;
    if (name_len <= 0 || name_len >= MAX_ALIAS_LEN) {
        terminal_write("error: alias name too long\n");
        return;
    }

    char name[MAX_ALIAS_LEN];
    strncpy(name, args, name_len);
    name[name_len] = '\0';

    const char* command = equal_pos + 1;
    if (!*command) {
        terminal_write("error: alias command missing\n");
        return;
    }

    if (alias_set(name, command) == 0) {
        terminal_write("Alias set.\n");
    } else {
        terminal_write("error: could not set alias (maybe full)\n");
    }
}

void cmd_unalias(const char* args) {
    if (!args || !*args) {
        terminal_write("error: missing alias name to remove\n");
        return;
    }

    if (alias_unset(args) == 0) {
        terminal_write("Alias removed.\n");
    } else {
        terminal_write("error: alias not found\n");
    }
}
