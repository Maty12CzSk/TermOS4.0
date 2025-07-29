#include "core/commands.h"
#include "core/io.h"
#include "core/colors.h"
#include "core/utils.h"
#include "core/shell.h"

void cmd_color(const char* args) {
    int c = simple_atoi(args);
    if (c >= 0 && c <= 15) {
        set_fg_color((uint8_t)c);
        terminal_write("Color changed.\n");
    } else {
        terminal_write("Usage: color <0-15>\n");
    }
}
