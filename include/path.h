#ifndef PATH_H
#define PATH_H

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#ifndef PATH_EMPTY
#define PATH_EMPTY 1
#endif

typedef struct {
    char **dirs;
    int tdirs;
} PATH_t;

extern PATH_t *path;

PATH_t *load_path();
char *find_in_path(PATH_t *path, char *bin);
void freepath(PATH_t *path);

#endif // PATH_H
