#ifndef BUILT_INS_H
#define BUILT_INS_H

#include <linux/limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

#include "./command_list.h"

#define OPEN_ERR 2

int builtin(char *cmd);
int cat(char **files, int tfiles);
int date();
int search_in(char *dir, char *file);
bool isexecutable(char *cmd);
int type(char *cmd);

#endif // !BUILT_INS_H
