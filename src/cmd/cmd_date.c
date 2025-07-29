#include "core/commands.h"
#include "core/io.h"
#include "core/shell.h"

void cmd_date(void) {
    terminal_write("24.July.2023 12:34:56\n");
    terminal_write("(For now its just static)\n");
}
