#ifndef COLORS_H
#define COLORS_H

#include <stdint.h>

extern uint8_t term_color;

#define COLOR_CYAN 0x03
void set_fg_color(uint8_t fg);
void set_bg_color(uint8_t bg);
void reset_color();
void set_color(uint8_t fg, uint8_t bg);
uint8_t get_term_color();
void cmd_rainbow(const char* text);

#endif