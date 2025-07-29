#include "core/shell.h"
#include "core/keymapping.h"
#include <stdint.h>
#include "core/utils.h"
#include "core/io.h"
#include "core/colors.h"
#include "core/history.h"
#include <string.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
volatile uint16_t* vga_buffer = (uint16_t*)0xB8000;
uint8_t term_row = 0, term_col = 0;

static inline uint16_t vga_entry(char c, uint8_t color) {
    return (uint16_t)c | (uint16_t)color << 8;
}

void terminal_scroll() {
    uint8_t color = get_term_color();
    for (size_t y = 1; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            vga_buffer[(y - 1) * VGA_WIDTH + x] = vga_buffer[y * VGA_WIDTH + x];
        }
    }
    for (size_t x = 0; x < VGA_WIDTH; x++) {
        vga_buffer[(VGA_HEIGHT - 1) * VGA_WIDTH + x] = vga_entry(' ', color);
    }
}

void terminal_goto(int row, int col) {
    unsigned short pos = row * VGA_WIDTH + col;
    outb(0x3D4, 0x0F);
    outb(0x3D5, (unsigned char)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (unsigned char)((pos >> 8) & 0xFF));
}

void terminal_update_cursor() {
    uint16_t pos = term_row * VGA_WIDTH + term_col;
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

void terminal_clear() {
    uint8_t color = get_term_color();
    for (size_t y = 0; y < VGA_HEIGHT; y++)
        for (size_t x = 0; x < VGA_WIDTH; x++)
            vga_buffer[y * VGA_WIDTH + x] = vga_entry(' ', color);
    term_row = 0; term_col = 0;
    terminal_update_cursor();
}

// All indices size_t (unsigned), avoid signed/unsigned comparison
void terminal_putchar(char c) {
    uint8_t color = get_term_color();
    if (c == '\n') {
        term_col = 0;
        if (++term_row == VGA_HEIGHT) {
            terminal_scroll();
            term_row = VGA_HEIGHT - 1;
        }
        terminal_update_cursor();
        return;
    } else if (c == '\b') {
        if (term_col > 0) {
            term_col--;
        } else if (term_row > 0) {
            term_row--;
            term_col = VGA_WIDTH - 1;
        }
        vga_buffer[term_row * VGA_WIDTH + term_col] = vga_entry(' ', color);
        terminal_update_cursor();
        return;
    }
    vga_buffer[term_row * VGA_WIDTH + term_col] = vga_entry(c, color);
    if (++term_col == VGA_WIDTH) {
        term_col = 0;
        if (++term_row == VGA_HEIGHT) {
            terminal_scroll();
            term_row = VGA_HEIGHT - 1;
        }
    }
    terminal_update_cursor();
}

void terminal_write(const char* str) {
    for (; *str; ++str) terminal_putchar(*str);
}

// Helper to clear the whole input line (prompt + input buffer)
void terminal_clear_line(const char* prompt, size_t input_row) {
    term_row = input_row;
    term_col = 0;
    terminal_update_cursor();

    size_t total_len = strlen(prompt) + CMD_BUF_LEN;
    for (size_t i = 0; i < total_len && i < VGA_WIDTH; ++i) {
        vga_buffer[input_row * VGA_WIDTH + i] = vga_entry(' ', get_term_color());
    }

    term_col = 0;
    terminal_update_cursor();
}

void terminal_readline(char* buf, size_t maxlen) {
    size_t i = 0;
    int shift = 0;
    char prompt[] = "TermOS4.1> ";
    buf[0] = 0;
    history_reset_pos();

    terminal_write(prompt);
    size_t input_row = term_row;

    uint8_t prev_sc = 0;
    int editing_input = 1;

    size_t cursor_pos = 0; // Now size_t, matches i

    while (1) {
        uint8_t sc;
        while (!(inb(0x64) & 1));
        sc = inb(0x60);

        // Shift keys
        if (sc == 0x2A || sc == 0x36) { shift = 1; prev_sc = sc; continue; }
        if (sc == 0xAA || sc == 0xB6) { shift = 0; prev_sc = sc; continue; }
        if (sc & 0x80) { prev_sc = sc; continue; }

        // History recall
        if (editing_input && prev_sc == 0xE0) {
            if (sc == SC_UP_ARROW) {
                const char* recalled = history_prev();
                terminal_clear_line(prompt, input_row);
                terminal_write(prompt);
                if (recalled && recalled[0]) {
                    strncpy(buf, recalled, maxlen - 1);
                    buf[maxlen - 1] = 0;
                    i = strlen(buf);
                    cursor_pos = i;
                    terminal_write(buf);
                } else {
                    buf[0] = 0;
                    i = 0;
                    cursor_pos = 0;
                }
                terminal_goto(input_row, strlen(prompt) + cursor_pos);
                prev_sc = 0;
                continue;
            } else if (sc == SC_DOWN_ARROW) {
                const char* recalled = history_next();
                terminal_clear_line(prompt, input_row);
                terminal_write(prompt);
                if (recalled && recalled[0]) {
                    strncpy(buf, recalled, maxlen - 1);
                    buf[maxlen - 1] = 0;
                    i = strlen(buf);
                    cursor_pos = i;
                    terminal_write(buf);
                } else {
                    buf[0] = 0;
                    i = 0;
                    cursor_pos = 0;
                }
                terminal_goto(input_row, strlen(prompt) + cursor_pos);
                prev_sc = 0;
                continue;
            }
        }

        // LEFT/RIGHT ARROW SUPPORT
        if (prev_sc == 0xE0) {
            if (sc == SC_LEFT_ARROW) {
                if (cursor_pos > 0) {
                    cursor_pos--;
                    terminal_goto(input_row, strlen(prompt) + cursor_pos);
                }
                prev_sc = 0;
                continue;
            } else if (sc == SC_RIGHT_ARROW) {
                if (cursor_pos < i) {
                    cursor_pos++;
                    terminal_goto(input_row, strlen(prompt) + cursor_pos);
                }
                prev_sc = 0;
                continue;
            }
            prev_sc = 0;
            continue;
        }

        if (sc == 0xE0) { prev_sc = sc; continue; }

        char c = shift ? kbdmap_shift[sc] : kbdmap[sc];
        if (c) {
            if (c == 13) { // Enter
                terminal_putchar('\n');
                buf[i] = 0;
                history_add(buf);
                editing_input = 0;
                return;
            } else if (c == 8) { // Backspace
                if (cursor_pos > 0 && i > 0) {
                    // Use size_t for j; make sure j < i-1 is always safe
                    for (size_t j = cursor_pos - 1; j + 1 < i; ++j)
                        buf[j] = buf[j + 1];
                    i--;
                    cursor_pos--;
                    buf[i] = 0;
                    terminal_clear_line(prompt, input_row);
                    terminal_write(prompt);
                    terminal_write(buf);
                    terminal_putchar(' ');
                    terminal_goto(input_row, strlen(prompt) + cursor_pos);
                }
            } else if (i < maxlen - 1) {
                for (size_t j = i; j > cursor_pos; --j)
                    buf[j] = buf[j-1];
                buf[cursor_pos] = c;
                i++;
                cursor_pos++;
                buf[i] = 0;
                terminal_clear_line(prompt, input_row);
                terminal_write(prompt);
                terminal_write(buf);
                terminal_goto(input_row, strlen(prompt) + cursor_pos);
            }
        }
        prev_sc = sc;
    }
}