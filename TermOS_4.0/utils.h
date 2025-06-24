#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>

char* int_to_str(int n, char* buf);
#define CMD_BUF_LEN 128
void run_commands_separated(const char* line);
int strcmp(const char* a, const char* b);
size_t strlen(const char* s);
int strncmp(const char* a, const char* b, size_t n);
void *memcpy(void *dest, const void *src, size_t n);
void *memset(void *s, int c, size_t n);
char *strncpy(char *dst, const char *src, size_t n);
char *strstr(const char *haystack, const char *needle);
// Simple atoi for kernel (base 10 only, no error checking)
int simple_atoi(const char* s);

#endif