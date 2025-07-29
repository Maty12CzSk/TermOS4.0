#include "core/commands.h"
#include "core/io.h"
#include "core/shell.h"

void cmd_reboot(void) {
    terminal_clear();
    terminal_write("Rebooting system...\n");
    for (volatile int d = 0; d < 150000000; ++d);
    __asm__ __volatile__("cli");
    while (inb(0x64) & 0x02);
    outb(0x64, 0xFE);
    for (;;) __asm__ __volatile__("hlt");
}
