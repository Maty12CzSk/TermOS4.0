#include "core/commands.h"
#include "core/io.h"
#include "core/shell.h"

void cmd_beep(void) {
    terminal_write("Beep!\n");
}
