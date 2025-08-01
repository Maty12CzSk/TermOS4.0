#include "commands.h"
#include "shell.h"
#include "ramfs.h"
#include "utils.h"
#include "colors.h"
#include "io.h"
#include "keymapping.h"
#include "nano.h"      // for nano_open

// Forward declare if needed
void run_script(const char* filename);

void handle_command(const char* cmd) {
    if (cmd[0] == 0) return;

    if (!strncmp(cmd, "source ", 7) || !strncmp(cmd, "run ", 4)) {
        const char *name = cmd + (cmd[0] == 's' ? 7 : 4);
        while (*name == ' ') ++name;
        run_script(name);
        return;
    }

    if (!strcmp(cmd, "ls")) {
        int found = 0;
        for (int i = 0; i < MAX_FILES; ++i) {
            RAMFile* file = ramfs_file_at(i);
            if (file) {
                terminal_write(file->name);
                terminal_putchar('\n');
                found = 1;
            }
        }
        if (!found) terminal_write("No files.\n");
        return;
    }
    if (!strncmp(cmd, "cat ", 4)) {
        const char *name = cmd + 4;
        int idx = ramfs_find(name);
        if (idx == -1) {
            terminal_write("No such file.\n");
        } else {
            RAMFile* f = ramfs_file_at(idx);
            terminal_write(f->data);
            if (f->size > 0 && f->data[f->size - 1] != '\n')
                terminal_putchar('\n');
        }
        return;
    }
    if (!strncmp(cmd, "touch ", 6)) {
        const char *name = cmd + 6;
        if (strlen(name) == 0) {
            terminal_write("touch: missing filename\n");
            return;
        }
        if (ramfs_create(name) == -1) {
            terminal_write("touch: cannot create file\n");
        }
        return;
    }
    if (!strncmp(cmd, "rm ", 3)) {
        const char *name = cmd + 3;
        int idx = ramfs_find(name);
        if (idx == -1) {
            terminal_write("rm: no such file\n");
        } else {
            ramfs_delete(name);
        }
        return;
    }
    if (!strncmp(cmd, "echo ", 5)) {
        const char *p = cmd + 5;
        const char *redir = strstr(p, " > ");
        const char *append = strstr(p, " >> ");
        if (redir || append) {
            size_t textlen = (redir ? redir : append) - p;
            char text[128] = {0};
            if (textlen > 127) textlen = 127;
            memcpy(text, p, textlen);
            text[textlen] = 0;
            const char *fname = (redir ? redir + 3 : append + 4);
            while (*fname == ' ') fname++;
            if (strlen(fname) == 0) {
                terminal_write("echo: missing filename after >\n");
                return;
            }
            if (append)
                ramfs_append(fname, text);
            else
                ramfs_write(fname, text);
        } else {
            terminal_write(p);
            terminal_putchar('\n');
        }
        return;
    }

    // Add your built-in commands here (help, clear, about, etc.)
    if (!strcmp(cmd, "help")) {
        terminal_write(
            "Available commands:\n"
            "help     - show this help\n"
            "clear    - clear screen\n"
            "about    - about this OS\n"
            "ls\n"
            "cat <file>\n"
            "touch <file>\n"
            "rm <file>\n"
            "echo <txt> > <file>\n"
            "echo <txt> >> <file>\n"
            "echo     - print text (usage: echo hello)\n"
            "source <file> or run <file> - run script\n"
            "color    - change text color (usage: color <0-15>)\n"
            "reset    - reset terminal color to white\n"
            "banner   - show a banner\n"
            "whoami   - print user\n"
            "uptime   - system ticks since boot\n"
            "beep     - beep PC speaker\n"
            "shutdown - halt the CPU\n"
            "reboot   - (not implemented)\n"
            "nano <file> - edit a file\n"
        );
    } else if (!strcmp(cmd, "clear")) {
        terminal_clear();
    } else if (!strcmp(cmd, "about")) {
        terminal_write("TermOS 4.0 - Example x86 OS for GRUB, by Mia\n");
    } else if (!strcmp(cmd, "reset")) {
        reset_color();
        terminal_write("Text color reset to white.\n");
    } else if (!strcmp(cmd, "banner")) {
        terminal_write(
            "==============================\n"
            "   Welcome to TermOS 4.0!\n"
            "==============================\n"
        );
    } else if (!strcmp(cmd, "whoami")) {
        terminal_write("You are root (single user system)\n");
    } else if (!strcmp(cmd, "uptime")) {
        terminal_write("Uptime (ticks): [not implemented]\n");
    } else if (!strcmp(cmd, "beep")) {
        // implement beep
        terminal_write("Beep!\n");
    } else if (!strcmp(cmd, "shutdown")) {
        terminal_write("System halted.\n");
        for (;;) __asm__ __volatile__("cli; hlt");
    } else if (!strcmp(cmd, "reboot")) {
        terminal_write("Reboot not implemented.\n");
    } else if (!strncmp(cmd, "color ", 6)) {
        int c = simple_atoi(cmd + 6);
        if (c >= 0 && c <= 15) {
            set_fg_color((uint8_t)c);
            terminal_write("Color changed.\n");
        } else {
            terminal_write("Usage: color <0-15>\n");
        }
    } else if (!strncmp(cmd, "nano ", 5)) {
        const char* filename = cmd + 5;
        nano_open(filename);
        return;
    } else {
        terminal_write("Unknown command. Type 'help'.\n");
    }
}

// Script runner implementation (move from your former kernel.c)
void run_script(const char *filename) {
    int idx = ramfs_find(filename);
    if (idx == -1) {
        terminal_write("No such file.\n");
        return;
    }
    RAMFile* f = ramfs_file_at(idx);
    char buf[1024];
    size_t len = f->size < sizeof(buf)-1 ? f->size : sizeof(buf)-1;
    memcpy(buf, f->data, len);
    buf[len] = 0;

    char *line = buf;
    while (*line) {
        while (*line == '\r' || *line == '\n') ++line;
        if (!*line) break;
        char *next = line;
        while (*next && *next != '\n' && *next != '\r') ++next;
        char old = *next;
        *next = 0;

        char *end = line + strlen(line);
        while (end > line && (end[-1] == '\r' || end[-1] == '\n' || end[-1] == ' ' || end[-1] == '\t')) --end;
        *end = 0;

        if (line[0]) run_commands_separated(line);

        *next = old;
        if (*next) ++next;
        line = next;
    }
}