#include "core/commands.h"
#include "core/io.h"
#include "core/shell.h"

void cmd_banner(void) {
    terminal_write(
        "==============================\n"
        "   Welcome to TermOS 4.1!\n"
        "==============================\n"
    );
}
