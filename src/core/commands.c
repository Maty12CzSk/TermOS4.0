#include "core/commands.h"
#include "core/shell.h"
#include "fs/ramfs.h"
#include "core/utils.h"
#include "core/colors.h"
#include "core/io.h"
#include "core/keymapping.h"
extern RAMFile* current_dir;

void handle_command(const char* cmd) {
    if (!cmd || !*cmd) return;

    // Check if it's an alias
    const char* alias_value = alias_get(cmd);
    if (alias_value) {
        run_commands_separated(alias_value);
        return;
    }

    if (!strncmp(cmd, "source ", 7) || !strncmp(cmd, "run ", 4)) {
        const char *name = cmd + (cmd[0] == 's' ? 7 : 4);
        while (*name == ' ') ++name;
        run_script(name);
        return;
    }
    if (!strcmp(cmd, "rand")) {
        cmd_rand();
        return;
    }
    if (!strcmp(cmd, "ls")){
        cmd_ls();
        return;
    }
    if (strncmp(cmd, "cmp ", 4) == 0) {
        const char* args = cmd + 4;
        while (*args == ' ') ++args;
        cmd_cmp(args);
        return;
    }
    if (cmd[0] == '!' && cmd[1] != 0) {
        cmd_repeat(cmd + 1);
        return;
    }
    if (!strncmp(cmd, "truncate ", 9)) {
        const char* name = cmd + 9;
        while (*name == ' ') ++name;
        truncate_command(name);
        return;
    }
    if (!strcmp(cmd, "history")){
        cmd_history();
        return;
    }
    if (!strncmp(cmd, "rename ", 7)) {
        const char* args = cmd + 7;
        cmd_rename(args);
        return;
    }
    if (!strncmp(cmd, "mv ", 3)) {
        const char* args = cmd + 3;
        cmd_mv(args);
        return;
    }
    if (!strncmp(cmd, "alias ", 6)) {
        const char* args = cmd + 6;
        cmd_alias(args);
        return;
    }
    if (!strcmp(cmd, "alias")) {
        cmd_alias(NULL);
        return;
    }
    if (!strncmp(cmd, "unalias ", 8)) {
        const char* args = cmd + 8;
        cmd_unalias(args);
        return;
    }

    if (!strcmp(cmd, "pwd")) {
        cmd_pwd();
        return;
    }
    if (!strncmp(cmd, "cat ", 4)) {
        const char* name = cmd + 4;
        cmd_cat(name);
        return;
    }
    if (!strncmp(cmd, "touch ", 6)) {
        const char* name = cmd + 6;
        cmd_touch(name);
        return;
    }
    if (!strncmp(cmd, "mkdir ", 6)) {
        const char* name = cmd + 6;
        cmd_mkdir(name);
        return;
    }
    if (!strncmp(cmd, "rm ", 3)) {
        const char* name = cmd + 3;
        cmd_rm(name);
        return;
    }
    if (!strncmp(cmd, "cd ", 3)) {
        const char* name = cmd + 3;
        cmd_cd(name);
        return;
    }
    if (!strcmp(cmd, "cd")) {
        cmd_cd("");
        return;
    }
    if (!strncmp(cmd, "echo ", 5)) {
        const char* args = cmd + 5;
        cmd_echo(args);
        return;
    }
    if (!strcmp(cmd, "help")) {
        cmd_help();
        return;
    }
    if (!strcmp(cmd, "clear")) {
        cmd_clear();
        return;
    }
    if (!strcmp(cmd, "about")) {
        cmd_about();
        return;
    }
    if (!strcmp(cmd, "reset")) {
        cmd_reset();
        return;
    }
    if (!strcmp(cmd, "banner")) {
        cmd_banner();
        return;
    }
    if (!strcmp(cmd, "whoami")) {
        cmd_whoami();
        return;
    }
    if (!strcmp(cmd, "uptime")) {
        cmd_uptime();
        return;
    }
    if (!strcmp(cmd, "beep")) {
        cmd_beep();
        return;
    }
    if (!strcmp(cmd, "shutdown")) {
        cmd_shutdown();
        return;
    }
    if (!strcmp(cmd, "reboot")) {
        cmd_reboot();
        return;
    }
    if (!strncmp(cmd, "color ", 6)) {
        const char* args = cmd + 6;
        cmd_color(args);
        return;
    }
    if (!strncmp(cmd, "nano ", 5)) {
        const char* filename = cmd + 5;
        nano_open(filename);
        return;
    }
    if (!strncmp(cmd, "cp ", 3)) {
        const char* args = cmd + 3;
        cmd_cp(args);
        return;
    }
    if (!strncmp(cmd, "date", 4)) {
        cmd_date();
        return;
    }
    if (!strncmp(cmd, "time", 4)) {
        cmd_time();
        return;
    }
    if (!strncmp(cmd, "stat ", 5)) {
        const char* name = cmd + 5;
        while (*name == ' ') name++;
        cmd_stat(name);
        return;
    }

    if (!strncmp(cmd, "read", 4)) {
        cmd_read(cmd);
        return;
    }

    if (!strncmp(cmd, "rainbow ", 8)) {
        const char* text = cmd + 8;
        cmd_rainbow(text);
        return;
    }

    terminal_write("Unknown command. Type 'help'.\n");
}
