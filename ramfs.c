#include "ramfs.h"

// Use the minimal string/memory functions defined in kernel.c
extern int strcmp(const char* a, const char* b);
extern size_t strlen(const char* s);
extern void *memcpy(void *dest, const void *src, size_t n);
extern void *memset(void *s, int c, size_t n);
extern char *strncpy(char *dst, const char *src, size_t n);

/* ==== RAM-disk implementation ==== */
static RAMFile ramdisk[MAX_FILES];

void ramfs_init(void) {
    for (int i = 0; i < MAX_FILES; ++i)
        ramdisk[i].used = 0;
}

int ramfs_find(const char *name) {
    for (int i = 0; i < MAX_FILES; ++i)
        if (ramdisk[i].used && !strcmp(ramdisk[i].name, name))
            return i;
    return -1;
}

int ramfs_create(const char *name) {
    if (ramfs_find(name) != -1) return -1; // already exists
    for (int i = 0; i < MAX_FILES; ++i) {
        if (!ramdisk[i].used) {
            memset(ramdisk[i].name, 0, MAX_FILENAME);
            strncpy(ramdisk[i].name, name, MAX_FILENAME - 1);
            ramdisk[i].size = 0;
            ramdisk[i].used = 1;
            return i;
        }
    }
    return -1; // no space
}

void ramfs_delete(const char *name) {
    int idx = ramfs_find(name);
    if (idx != -1) ramdisk[idx].used = 0;
}

void ramfs_write(const char *name, const char *data) {
    int idx = ramfs_find(name);
    if (idx == -1) idx = ramfs_create(name);
    if (idx != -1) {
        size_t len = strlen(data);
        if (len > MAX_FILESIZE - 1) len = MAX_FILESIZE - 1;
        memcpy(ramdisk[idx].data, data, len);
        ramdisk[idx].data[len] = 0;
        ramdisk[idx].size = len;
    }
}

void ramfs_append(const char *name, const char *data) {
    int idx = ramfs_find(name);
    if (idx == -1) idx = ramfs_create(name);
    if (idx != -1) {
        size_t datalen = strlen(data);
        size_t avail = MAX_FILESIZE - 1 - ramdisk[idx].size;
        if (datalen > avail) datalen = avail;
        memcpy(ramdisk[idx].data + ramdisk[idx].size, data, datalen);
        ramdisk[idx].size += datalen;
        ramdisk[idx].data[ramdisk[idx].size] = 0;
    }
}

RAMFile* ramfs_file_at(int idx) {
    if (idx < 0 || idx >= MAX_FILES) return NULL;
    if (!ramdisk[idx].used) return NULL;
    return &ramdisk[idx];
}