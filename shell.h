#ifndef SHELL_H
#define SHELL_H

#include <stddef.h>

void terminal_goto(int row, int col);
void terminal_clear();
void terminal_write(const char* str);
void terminal_putchar(char c);
void terminal_update_cursor();
void terminal_readline(char* buf, size_t maxlen);
void add_history(const char* cmd);

#endif