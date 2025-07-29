#include "core/alias.h"
#include <string.h>

Alias aliases[MAX_ALIASES];

int alias_set(const char* name, const char* command) {
    // First check if it already exists and update
    for (int i = 0; i < MAX_ALIASES; i++) {
        if (aliases[i].used && strcmp(aliases[i].name, name) == 0) {
            strncpy(aliases[i].command, command, MAX_COMMAND_LEN - 1);
            aliases[i].command[MAX_COMMAND_LEN - 1] = '\0';
            return 0;
        }
    }

    // Otherwise create a new one
    for (int i = 0; i < MAX_ALIASES; i++) {
        if (!aliases[i].used) {
            strncpy(aliases[i].name, name, MAX_ALIAS_LEN - 1);
            aliases[i].name[MAX_ALIAS_LEN - 1] = '\0';
            strncpy(aliases[i].command, command, MAX_COMMAND_LEN - 1);
            aliases[i].command[MAX_COMMAND_LEN - 1] = '\0';
            aliases[i].used = 1;
            return 0;
        }
    }

    return -1; // No space
}

int alias_unset(const char* name) {
    for (int i = 0; i < MAX_ALIASES; i++) {
        if (aliases[i].used && strcmp(aliases[i].name, name) == 0) {
            aliases[i].used = 0;
            return 0;
        }
    }
    return -1; // Not found
}

const char* alias_get(const char* name) {
    for (int i = 0; i < MAX_ALIASES; i++) {
        if (aliases[i].used && strcmp(aliases[i].name, name) == 0) {
            return aliases[i].command;
        }
    }
    return NULL;
}
