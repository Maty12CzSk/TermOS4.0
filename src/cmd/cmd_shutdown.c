#include "core/commands.h"
#include "core/io.h"
#include "core/shell.h"

void cmd_shutdown(void) {
    terminal_write("System halted.\n");
    for (;;) __asm__ __volatile__("cli; hlt");
}
