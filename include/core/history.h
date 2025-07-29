#ifndef HISTORY_H
#define HISTORY_H

#include <stddef.h>

#define HISTORY_SIZE 16
#define CMD_BUF_LEN 128

void history_init(void);
void history_add(const char* cmd);
const char* history_prev(void);
const char* history_next(void);
void history_reset_pos(void);

const char* history_get(int index);
int history_size(void);


#endif