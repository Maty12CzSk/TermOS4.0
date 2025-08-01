#include "colors.h"

uint8_t term_color = 0x0F; // Default: white on black

void set_fg_color(uint8_t fg) {
    term_color = (term_color & 0xF0) | (fg & 0x0F);
}

void set_bg_color(uint8_t bg) {
    term_color = (term_color & 0x0F) | ((bg & 0x0F) << 4);
}

void set_color(uint8_t fg, uint8_t bg) {
    term_color = ((bg & 0x0F) << 4) | (fg & 0x0F);
}

void reset_color() {
    term_color = 0x0F; // White on black
}

uint8_t get_term_color() {
    return term_color;
}