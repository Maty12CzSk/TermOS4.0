// cmd_history.c
#include "core/history.h"
#include "core/shell.h"
#include <stddef.h>

static void int_to_str(int num, char* buf, int bufsize) {
    if (bufsize < 3) return; // buffer too small

    if (num < 10) {
        buf[0] = '0' + num;
        buf[1] = 0;
    } else {
        buf[0] = '0' + (num / 10);
        buf[1] = '0' + (num % 10);
        buf[2] = 0;
    }
}

// writes command history
void cmd_history(void) {
    int count = history_size();
    for (int i = 0; i < count; i++) {
        const char* cmd = history_get(i);
        if (!cmd) continue;

        char line[CMD_BUF_LEN + 8];
        int pos = 0;

        char numbuf[3];
        int_to_str(i + 1, numbuf, sizeof(numbuf));
        for (int j = 0; numbuf[j] != 0 && pos < (int)sizeof(line)-1; j++)
            line[pos++] = numbuf[j];

        line[pos++] = ' ';
        line[pos++] = ' ';

        // add command
        for (int j = 0; cmd[j] != 0 && pos < (int)sizeof(line)-1; j++)
            line[pos++] = cmd[j];

        // add new line
        if (pos < (int)sizeof(line)-1) line[pos++] = '\n';

        line[pos] = 0;

        terminal_write(line);
    }
}
