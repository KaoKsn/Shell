#ifndef BUILT_INS_H
#define BUILT_INS_H

#include <errno.h>
#include <linux/limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#include "./command_list.h"

#define OPEN_ERR 2
#define HOST_BUFF_MAX 256

bool isexecutable(char *cmd);
int search_in(char *dir, char *file);
int builtin(char *cmd);

int cat(char **files, int tfiles);
int date();
int pwd();
int type(char *cmd);

#endif // !BUILT_INS_H
