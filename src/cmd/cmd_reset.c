#include "core/commands.h"
#include "core/io.h"
#include "core/colors.h"
#include "core/shell.h"

void cmd_reset(void) {
    reset_color();
    terminal_write("Text color reset to white.\n");
}
