#include "core/commands.h"
#include "core/io.h"
#include "core/shell.h"

void cmd_whoami(void) {
    terminal_write("You are root (single user system)\n");
}
