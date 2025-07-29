#include "core/commands.h"
#include "core/io.h"
#include "core/shell.h"

void cmd_time(void) {
    terminal_write("12:34:56\n");
    terminal_write("(For now its just static)\n");
}
