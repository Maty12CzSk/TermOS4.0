#pragma once
#include <stddef.h>

#define MAX_FILES 16
#define MAX_FILENAME 16
#define MAX_FILESIZE 256

typedef struct {
    char name[MAX_FILENAME];
    char data[MAX_FILESIZE];
    size_t size;
    int used; // 0 = free, 1 = used
} RAMFile;

void ramfs_init(void);
int ramfs_find(const char *name);
int ramfs_create(const char *name);
void ramfs_delete(const char *name);
void ramfs_write(const char *name, const char *data);
void ramfs_append(const char *name, const char *data);
RAMFile* ramfs_file_at(int idx);