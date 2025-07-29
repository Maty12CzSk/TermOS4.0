#include "core/shell.h"
#include "core/commands.h"
#include "fs/ramfs.h"
#include "core/utils.h"
void kernel_main() {
    terminal_clear();
    terminal_write(
            "==============================\n"
            "   Welcome to TermOS 4.1!\n"
            "           by Mia         \n"
            "==============================\n"
            "   Type 'help' for commands.\n"
        );
    ramfs_init();

    char input[128];
    while (1) {
        terminal_readline(input, sizeof(input));
        if (input[0])
            handle_command(input);
    }
}