#include "core/commands.h"
#include "core/io.h"
#include "core/shell.h"

void cmd_clear(void) {
    terminal_clear();
}
