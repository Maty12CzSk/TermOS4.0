#include "core/colors.h"
#include "core/shell.h"
#include <string.h>

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

void cmd_rainbow(const char *text) {
    // Duhové barvy v pořadí
    const uint8_t rainbow_colors[] = {
        0x04, // red
        0x06, // brown / orange
        0x0E, // yellow
        0x0A, // green
        0x09, // blue
        0x0D, // pink / magenta
        0x05  // purple
    };
    const int num_colors = sizeof(rainbow_colors) / sizeof(rainbow_colors[0]);

    uint8_t original_color = get_term_color();

    for (int i = 0; text[i]; i++) {
        set_fg_color(rainbow_colors[i % num_colors]);
        terminal_putchar(text[i]);
    }

    reset_color();
    terminal_putchar('\n');
}
