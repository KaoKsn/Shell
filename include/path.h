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
} PATH;

extern PATH *path;

PATH *load_path();
char *find_in_path(PATH *path, char *bin);
void freepath(PATH *path);

#endif // PATH_H
