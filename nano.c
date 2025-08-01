#include "nano.h"
#include "shell.h"
#include "ramfs.h"
#include "utils.h"
#include "colors.h"
#include "io.h"
#include "keymapping.h"
#include <string.h>
#include <stdio.h>

#define NANO_MAX_LINES 128
#define NANO_MAX_LINE_LEN 128
#define NANO_SCREEN_ROWS 22
#define NANO_SCREEN_COLS 80

static char lines[NANO_MAX_LINES][NANO_MAX_LINE_LEN];
static int line_count = 0;
static int cx = 0, cy = 0;
static int row_offset = 0;
static int col_offset = 0;
static char filename_buf[64];
static int dirty = 0;

static void nano_status(const char* msg) {
    set_fg_color(COLOR_CYAN);
    terminal_goto(NANO_SCREEN_ROWS, 0);
    for (int i = 0; i < NANO_SCREEN_COLS; ++i)
        terminal_putchar(' ');
    terminal_goto(NANO_SCREEN_ROWS, 0);
    terminal_write(msg);
    reset_color();
}

static void nano_draw() {
    terminal_clear();
    for (int i = 0; i < NANO_SCREEN_ROWS; ++i) {
        int li = i + row_offset;
        terminal_goto(i, 0);
        if (li < line_count) {
            int len = strlen(lines[li]);
            for (int j = 0; j < NANO_SCREEN_COLS; ++j) {
                char c = (j + col_offset < len) ? lines[li][j + col_offset] : ' ';
                terminal_putchar(c);
            }
        } else {
            for (int j = 0; j < NANO_SCREEN_COLS; ++j)
                terminal_putchar(' ');
        }
    }

    char status[100];
    char numbuf1[12], numbuf2[12], numbuf3[12];
    strcpy(status, "nano - ");
    strcat(status, filename_buf);
    strcat(status, "  |  F2: save  |  ESC: exit  |  Ln: ");
    int_to_str(cy + 1, numbuf1);
    strcat(status, numbuf1);
    strcat(status, "/");
    int_to_str(line_count, numbuf2);
    strcat(status, numbuf2);
    strcat(status, "  Col: ");
    int_to_str(cx + 1, numbuf3);
    strcat(status, numbuf3);
    nano_status(status);

    int screen_y = cy - row_offset;
    int screen_x = cx - col_offset;
    if (screen_y < 0) screen_y = 0;
    if (screen_x < 0) screen_x = 0;
    terminal_goto(screen_y, screen_x);
}

static void nano_load(const char* filename) {
    memset(lines, 0, sizeof(lines));
    line_count = 1;
    strncpy(filename_buf, filename, sizeof(filename_buf) - 1);
    filename_buf[sizeof(filename_buf) - 1] = 0;
    int idx = ramfs_find(filename);
    if (idx == -1) return;
    RAMFile* f = ramfs_file_at(idx);
    int l = 0, p = 0;
    for (size_t i = 0; i < f->size && l < NANO_MAX_LINES; ++i) {
        char c = f->data[i];
        if (c == '\n' || c == '\r') {
            lines[l][p] = 0;
            l++;
            p = 0;
            if (l < NANO_MAX_LINES)
                line_count++;
            while (i+1 < f->size && (f->data[i+1] == '\n' || f->data[i+1] == '\r')) ++i;
        } else if (p < NANO_MAX_LINE_LEN - 1) {
            lines[l][p++] = c;
        }
    }
    if (l < NANO_MAX_LINES)
        lines[l][p] = 0;
    dirty = 0;
}

static void nano_save(const char* filename) {
    char buf[NANO_MAX_LINES * NANO_MAX_LINE_LEN + 1];
    int pos = 0;
    for (int i = 0; i < line_count; ++i) {
        int len = strlen(lines[i]);
        if ((size_t)(pos + len + 1) >= sizeof(buf)) break;
        memcpy(buf + pos, lines[i], len);
        pos += len;
        buf[pos++] = '\n';
    }
    buf[pos] = 0;
    ramfs_write(filename, buf);
    nano_status("[Saved]");
    dirty = 0;
    for (volatile int d = 0; d < 1500000; ++d);
}

static void nano_insert_char(char c) {
    int len = strlen(lines[cy]);
    if (len >= NANO_MAX_LINE_LEN - 1) return;
    if (cx > len) cx = len;
    for (int i = len; i >= cx; --i) lines[cy][i+1] = lines[cy][i];
    lines[cy][cx] = c;
    cx++;
    dirty = 1;
}

static void nano_backspace() {
    if (cx > 0) {
        int len = strlen(lines[cy]);
        for (int i = cx - 1; i < len; ++i)
            lines[cy][i] = lines[cy][i+1];
        cx--;
        dirty = 1;
    } else if (cy > 0) {
        int prev_len = strlen(lines[cy-1]);
        int this_len = strlen(lines[cy]);
        if (prev_len + this_len < NANO_MAX_LINE_LEN - 1) {
            strcat(lines[cy-1], lines[cy]);
            for (int i = cy; i < line_count - 1; ++i)
                strcpy(lines[i], lines[i+1]);
            line_count--;
            cy--;
            cx = prev_len;
            dirty = 1;
        }
    }
}

static void nano_delete_char() {
    int len = strlen(lines[cy]);
    if (cx < len) {
        for (int i = cx; i < len; ++i)
            lines[cy][i] = lines[cy][i+1];
        dirty = 1;
    } else if (cy + 1 < line_count) {
        int this_len = strlen(lines[cy]);
        int next_len = strlen(lines[cy+1]);
        if (this_len + next_len < NANO_MAX_LINE_LEN - 1) {
            strcat(lines[cy], lines[cy+1]);
            for (int i = cy+1; i < line_count - 1; ++i)
                strcpy(lines[i], lines[i+1]);
            line_count--;
            dirty = 1;
        }
    }
}

static void nano_newline() {
    if (line_count >= NANO_MAX_LINES) return;
    int len = strlen(lines[cy]);
    if (cx > len) cx = len;
    for (int i = line_count; i > cy + 1; --i)
        strcpy(lines[i], lines[i-1]);
    strcpy(lines[cy+1], lines[cy] + cx);
    lines[cy][cx] = 0;
    cy++;
    cx = 0;
    line_count++;
    dirty = 1;
}

// Use your own keymapping arrays (kbdmap/kbdmap_shift) for translating scancode + shift to ASCII char
static char scancode_to_char(uint8_t sc, int shift) {
    if (sc > 127) return 0;
    return shift ? kbdmap_shift[sc] : kbdmap[sc];
}

void nano_open(const char* filename) {
    nano_load(filename);
    cx = 0; cy = 0; row_offset = 0; col_offset = 0;
    nano_draw();

    int shift = 0;
    uint8_t prev_sc = 0;
    while (1) {
        uint8_t sc;
        while (!(inb(0x64) & 1));
        sc = inb(0x60);

        // Shift keys
        if (sc == 0x2A || sc == 0x36) { shift = 1; prev_sc = sc; continue; }
        if (sc == 0xAA || sc == 0xB6) { shift = 0; prev_sc = sc; continue; }
        if (sc & 0x80) { prev_sc = sc; continue; }

        // Handle arrows (after 0xE0)
        if (prev_sc == 0xE0) {
            if (sc == SC_LEFT_ARROW) {
                if (cx > 0) cx--;
                else if (cy > 0) {
                    cy--;
                    cx = strlen(lines[cy]);
                }
            } else if (sc == SC_RIGHT_ARROW) {
                if (cx < (int)strlen(lines[cy])) cx++;
                else if (cy + 1 < line_count) {
                    cy++;
                    cx = 0;
                }
            } else if (sc == SC_UP_ARROW) {
                if (cy > 0) {
                    cy--;
                    if (cx > (int)strlen(lines[cy])) cx = strlen(lines[cy]);
                }
            } else if (sc == SC_DOWN_ARROW) {
                if (cy + 1 < line_count) {
                    cy++;
                    if (cx > (int)strlen(lines[cy])) cx = strlen(lines[cy]);
                }
            } else if (sc == 0x47) { // Home
                cx = 0;
            } else if (sc == 0x4F) { // End
                cx = strlen(lines[cy]);
            }
            // Clamp scroll offsets
            if (cy < row_offset) row_offset = cy;
            if (cy >= row_offset + NANO_SCREEN_ROWS) row_offset = cy - NANO_SCREEN_ROWS + 1;
            if (cx < col_offset) col_offset = cx;
            if (cx >= col_offset + NANO_SCREEN_COLS) col_offset = cx - NANO_SCREEN_COLS + 1;
            nano_draw();
            prev_sc = 0;
            continue;
        }

        if (sc == 0xE0) { prev_sc = sc; continue; }

        // Exit/save/enter/backspace/delete
        if (sc == 0x01) {
            nano_status(dirty ? "[Exiting nano without saving]" : "[Exiting nano, all changes saved]");
            for (volatile int d = 0; d < 2000000; ++d);
            return;
        }
        if (sc == 0x3C) { nano_save(filename_buf); nano_draw(); continue; }
        if (sc == 0x1C) { nano_newline(); nano_draw(); continue; }
        if (sc == 0x0E) { nano_backspace(); nano_draw(); continue; }
        if (sc == 0x53 || sc == 0xD3) { nano_delete_char(); nano_draw(); continue; }

        // Insert printable char
        char ch = scancode_to_char(sc, shift);
        if (ch) {
            nano_insert_char(ch);
            nano_draw();
        }
        prev_sc = sc;
    }
}