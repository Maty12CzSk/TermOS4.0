#ifndef COMMANDS_H
#define COMMANDS_H

void handle_command(const char* cmd);

// commands
void truncate_command(const char* args);
void cmd_sysinfo(void);
void nano_open(const char* filename);
void cmd_cmp(const char* args);
void cmd_rename(const char* args);
void cmd_pwd(void);
void cmd_cat(const char* args);
void cmd_touch(const char* args);
void cmd_mkdir(const char* args);
void cmd_rm(const char* args);
void cmd_cd(const char* args);
void cmd_echo(const char* args);
void cmd_help(void);
void cmd_clear(void);
void cmd_about(void);
void cmd_reset(void);
void cmd_banner(void);
void cmd_whoami(void);
void cmd_ls(void);
void cmd_uptime(void);
void cmd_rand(void);
void cmd_beep(void);
void cmd_shutdown(void);
void cmd_reboot(void);
void cmd_color(const char* args);
void cmd_date(void);
void cmd_time(void);
void cmd_stat(const char* filename);
void cmd_read(const char* cmd);
void cmd_cp(const char* args);
void cmd_history(void);
void cmd_repeat(const char* args);
void cmd_mv(const char* args);
void cmd_alias(const char* args);
void cmd_unalias(const char* args);

// others utility
void run_script(const char* filename);
void run_commands_separated(const char* line);
void terminal_write_padded(const char* str, int width);

#endif
