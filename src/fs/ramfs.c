#include "fs/ramfs.h"
#include <string.h> // for memcpy, memset, strcmp, strlen

RAMFile root_dir[MAX_FILES];
RAMFile* current_dir = NULL;

void ramfs_init(void) {
    for (int i = 0; i < MAX_FILES; ++i) {
        root_dir[i].used = 0;
        root_dir[i].is_dir = 0;
        root_dir[i].parent = NULL;
        root_dir[i].child_count = 0;
        memset(root_dir[i].children, 0, sizeof(root_dir[i].children));
    }
    // Set the first entry as the root directory
    RAMFile* root = &root_dir[0];
    root->used = 1;
    root->is_dir = 1;
    root->parent = NULL;
    root->child_count = 0;
    current_dir = root;
}

int ramfs_find(const char* name) {
    for (int i = 0; i < MAX_FILES; ++i) {
        if (root_dir[i].used && strcmp(root_dir[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

RAMFile* ramfs_file_at(int idx) {
    if (idx < 0 || idx >= MAX_FILES) return NULL;
    if (!root_dir[idx].used) return NULL;
    return &root_dir[idx];
}

RAMFile* ramfs_find_in_dir(RAMFile* dir, const char* name) {
    if (!dir || !dir->is_dir) return NULL;
    for (int i = 0; i < dir->child_count; ++i) {
        RAMFile* child = dir->children[i];
        if (child && strcmp(child->name, name) == 0) {
            return child;
        }
    }
    return NULL;
}

RAMFile* ramfs_create_file(RAMFile* dir, const char* name) {
    if (!dir || !dir->is_dir) return NULL;
    if (ramfs_find_in_dir(dir, name)) return NULL; // already exists

    // Find a free slot in root_dir
    for (int i = 0; i < MAX_FILES; ++i) {
        if (!root_dir[i].used) {
            RAMFile* f = &root_dir[i];
            f->used = 1;
            f->is_dir = 0;
            strncpy(f->name, name, MAX_FILENAME - 1);
            f->name[MAX_FILENAME - 1] = 0;
            f->size = 0;
            f->parent = dir;
            f->child_count = 0;
            memset(f->children, 0, sizeof(f->children));

            // Add to parent directory
            if (dir->child_count < MAX_CHILDREN) {
                dir->children[dir->child_count++] = f;
            } else {
                // No space in parent, cancel creation
                f->used = 0;
                return NULL;
            }

            return f;
        }
    }
    return NULL; // no free slot
}

RAMFile* ramfs_create_dir(RAMFile* dir, const char* name) {
    if (!dir || !dir->is_dir) return NULL;
    if (ramfs_find_in_dir(dir, name)) return NULL; // already exists

    // Find a free slot in root_dir
    for (int i = 0; i < MAX_FILES; ++i) {
        if (!root_dir[i].used) {
            RAMFile* d = &root_dir[i];
            d->used = 1;
            d->is_dir = 1;
            strncpy(d->name, name, MAX_FILENAME - 1);
            d->name[MAX_FILENAME - 1] = 0;
            d->size = 0;
            d->parent = dir;
            d->child_count = 0;
            memset(d->children, 0, sizeof(d->children));

            // Add to parent directory
            if (dir->child_count < MAX_CHILDREN) {
                dir->children[dir->child_count++] = d;
            } else {
                d->used = 0;
                return NULL;
            }

            return d;
        }
    }
    return NULL; // no free slot
}

void ramfs_delete_file(RAMFile* file) {
    if (!file || !file->used) return;
    if (file->is_dir && file->child_count > 0) {
        // First delete all children (recursively)
        while (file->child_count > 0) {
            ramfs_delete_file(file->children[file->child_count - 1]);
        }
    }
    // Remove from parent
    if (file->parent) {
        RAMFile* parent = file->parent;
        int idx = -1;
        for (int i = 0; i < parent->child_count; ++i) {
            if (parent->children[i] == file) {
                idx = i;
                break;
            }
        }
        if (idx != -1) {
            // Shift array left
            for (int j = idx; j < parent->child_count - 1; ++j) {
                parent->children[j] = parent->children[j + 1];
            }
            parent->child_count--;
        }
    }

    // Reset file/directory
    file->used = 0;
    file->parent = NULL;
    file->child_count = 0;
    memset(file->children, 0, sizeof(file->children));
}

void ramfs_write(RAMFile* file, const char* data) {
    if (!file || !file->used || file->is_dir) return;
    size_t len = strlen(data);
    if (len > MAX_FILESIZE - 1) len = MAX_FILESIZE - 1;
    memcpy(file->data, data, len);
    file->data[len] = 0;
    file->size = len;
}

void ramfs_append(RAMFile* file, const char* data) {
    if (!file || !file->used || file->is_dir) return;
    size_t datalen = strlen(data);
    size_t avail = MAX_FILESIZE - 1 - file->size;
    if (datalen > avail) datalen = avail;
    memcpy(file->data + file->size, data, datalen);
    file->size += datalen;
    file->data[file->size] = 0;
}

int ramfs_truncate(const char* path) {
    if (!path || path[0] == '\0') return -1;

    RAMFile* current = (path[0] == '/') ? &root_dir[0] : current_dir;
    int start = (path[0] == '/') ? 1 : 0;

    char token[MAX_FILENAME];
    int pos = 0;

    for (int i = start;; i++) {
        char c = path[i];
        if (c == '/' || c == '\0') {
            if (pos == 0) {
                if (c == '\0') break;
                else continue; // ignore repeated slashes
            }
            token[pos] = '\0';

            // Search token among children
            if (!current->is_dir) return -3; // not a directory

            RAMFile* next = NULL;
            for (int j = 0; j < current->child_count; j++) {
                if (strcmp(current->children[j]->name, token) == 0) {
                    next = current->children[j];
                    break;
                }
            }
            if (!next) return -2; // not found

            current = next;
            pos = 0;

            if (c == '\0') break;
        } else {
            if (pos < MAX_FILENAME - 1) token[pos++] = c;
        }
    }

    if (current->is_dir) return -3; // cannot truncate a directory

    memset(current->data, 0, MAX_FILESIZE);
    current->size = 0;

    return 0;
}
