#ifndef ALIAS_H
#define ALIAS_H

#define MAX_ALIASES 32
#define MAX_ALIAS_LEN 32
#define MAX_COMMAND_LEN 128

typedef struct {
    char name[MAX_ALIAS_LEN];
    char command[MAX_COMMAND_LEN];
    int used;
} Alias;

extern Alias aliases[MAX_ALIASES];

int alias_set(const char* name, const char* command);
int alias_unset(const char* name);
const char* alias_get(const char* name);

#endif
