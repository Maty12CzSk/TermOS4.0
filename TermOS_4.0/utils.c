#include "utils.h"

void handle_command(const char* cmd);
char *strcpy(char *dst, const char *src) {
    char *ret = dst;
    while ((*dst++ = *src++));
    return ret;
}

char *strcat(char *dst, const char *src) {
    char *ret = dst;
    while (*dst) ++dst;
    while ((*dst++ = *src++));
    return ret;
}
char* int_to_str(int n, char* buf) {
    char tmp[12];
    int i = 0, j = 0;
    if (n == 0) { buf[0] = '0'; buf[1] = 0; return buf; }
    int neg = 0;
    if (n < 0) { neg = 1; n = -n; }
    while (n > 0) { tmp[i++] = '0' + (n % 10); n /= 10; }
    if (neg) tmp[i++] = '-';
    while (i > 0) buf[j++] = tmp[--i];
    buf[j] = 0;
    return buf;
}
void run_commands_separated(const char* line) {
    const char *cmd_start = line;
    while (*cmd_start) {
        // skip leading spaces and separators
        while (*cmd_start == ' ' || *cmd_start == ';') ++cmd_start;
        if (!*cmd_start) break;
        const char *cmd_end = cmd_start;
        while (*cmd_end && *cmd_end != ';') ++cmd_end;
        size_t len = cmd_end - cmd_start;
        if (len > 0) {
            char onecmd[128] = {0};
            if (len > sizeof(onecmd) - 1) len = sizeof(onecmd) - 1;
            memcpy(onecmd, cmd_start, len);
            onecmd[len] = 0;
            handle_command(onecmd);
        }
        cmd_start = cmd_end;
        if (*cmd_start == ';') ++cmd_start;
    }
}

int strcmp(const char* a, const char* b) {
    while (*a && (*a == *b)) { a++; b++; }
    return *(const unsigned char*)a - *(const unsigned char*)b;
}

size_t strlen(const char* s) {
    size_t i = 0;
    while (s[i]) i++;
    return i;
}

int strncmp(const char* a, const char* b, size_t n) {
    for (size_t i = 0; i < n; i++) {
        if (a[i] != b[i] || !a[i] || !b[i]) return (unsigned char)a[i] - (unsigned char)b[i];
    }
    return 0;
}

void *memcpy(void *dest, const void *src, size_t n) {
    unsigned char *d = dest;
    const unsigned char *s = src;
    for (size_t i = 0; i < n; ++i) d[i] = s[i];
    return dest;
}

void *memset(void *s, int c, size_t n) {
    unsigned char *p = s;
    for (size_t i = 0; i < n; ++i) p[i] = (unsigned char)c;
    return s;
}

char *strncpy(char *dst, const char *src, size_t n) {
    size_t i;
    for (i = 0; i < n && src[i]; ++i) dst[i] = src[i];
    for (; i < n; ++i) dst[i] = 0;
    return dst;
}

char *strstr(const char *haystack, const char *needle) {
    if (!*needle) return (char *)haystack;
    size_t needle_len = strlen(needle);
    for (; *haystack; ++haystack) {
        if (!strncmp(haystack, needle, needle_len)) return (char *)haystack;
    }
    return NULL;
}

// Simple atoi for kernel (base 10 only, no error checking)
int simple_atoi(const char* s) {
    int n = 0;
    while (*s >= '0' && *s <= '9') {
        n = n * 10 + (*s - '0');
        s++;
    }
    return n;
}