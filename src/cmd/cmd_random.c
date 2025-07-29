#include "core/commands.h"
#include "core/shell.h"
#include "core/utils.h"

static unsigned int rand_seed = 1;

unsigned int rand() {
    rand_seed = rand_seed * 1103515245 + 12345;
    return (rand_seed / 65536) % 32768;
}

void cmd_rand(void) {
    unsigned int r = rand();
    char buf[16];
    int_to_str(r, buf);
    terminal_write(buf);
    terminal_putchar('\n');
}
