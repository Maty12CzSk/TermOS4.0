#pragma once
#include <stddef.h>

#define MAX_FILES 16
#define MAX_FILENAME 16
#define MAX_FILESIZE 256
#define MAX_CHILDREN 8
#define MAX_NAME_LEN 16
#define MAX_PATH_DEPTH 32


typedef struct RAMFile {
    char name[MAX_FILENAME];
    char data[MAX_FILESIZE];
    size_t size;
    int used;       // 0 = free, 1 = used
    int is_dir;     // 1 = directory, 0 = file
    struct RAMFile* parent;
    struct RAMFile* children[MAX_CHILDREN];
    int child_count;
} RAMFile;

extern RAMFile root_dir[MAX_FILES];
extern RAMFile* current_dir;

void ramfs_init(void);

int ramfs_find(const char* name);
RAMFile* ramfs_file_at(int idx);
int ramfs_truncate(const char* path);

RAMFile* ramfs_find_in_dir(RAMFile* dir, const char* name);
RAMFile* ramfs_create_file(RAMFile* dir, const char* name);
RAMFile* ramfs_create_dir(RAMFile* dir, const char* name);
void ramfs_delete_file(RAMFile* file);

void ramfs_write(RAMFile* file, const char* data);
void ramfs_append(RAMFile* file, const char* data);