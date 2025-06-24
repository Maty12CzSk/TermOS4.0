#include "history.h"
#include <string.h>

static char history[HISTORY_SIZE][CMD_BUF_LEN];
static int history_count = 0;
static int history_pos = 0; // Points one past the most recent entry when not navigating

void history_init(void) {
    history_count = 0;
    history_pos = 0;
    for (int i = 0; i < HISTORY_SIZE; ++i)
        history[i][0] = 0;
}

void history_add(const char* cmd) {
    if (!cmd || !cmd[0]) return;
    if (history_count > 0 && strncmp(cmd, history[history_count-1], CMD_BUF_LEN) == 0) return;
    if (history_count < HISTORY_SIZE) {
        strncpy(history[history_count], cmd, CMD_BUF_LEN - 1);
        history[history_count][CMD_BUF_LEN - 1] = 0;
        history_count++;
    } else {
        for (int i = 1; i < HISTORY_SIZE; ++i)
            memcpy(history[i - 1], history[i], CMD_BUF_LEN);
        strncpy(history[HISTORY_SIZE - 1], cmd, CMD_BUF_LEN - 1);
        history[HISTORY_SIZE - 1][CMD_BUF_LEN - 1] = 0;
    }
    history_pos = history_count; // Reset navigation after new command
}

const char* history_prev(void) {
    if (history_count == 0) return NULL;
    if (history_pos > 0) history_pos--;
    return history[history_pos];
}

const char* history_next(void) {
    if (history_count == 0) return NULL;
    if (history_pos < history_count - 1) {
        history_pos++;
        return history[history_pos];
    }
    history_pos = history_count;
    return "";
}

void history_reset_pos(void) {
    history_pos = history_count;
}