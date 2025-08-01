#include "shell.h"
#include "commands.h"
#include "ramfs.h"
#include "utils.h"
void kernel_main() {
    terminal_clear();
    terminal_write(
            "==============================\n"
            "   Welcome to TermOS 4.0!\n"
            "       by Yandere_Mia       \n"
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