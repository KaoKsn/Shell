#ifndef BUILT_INS_H
#define BUILT_INS_H

#include <stdio.h>
#include <time.h>

#define OPEN_ERR 2

int cat(char **files, int tfiles);
int date();
int type(char *cmd);

#endif // !BUILT_INS_H
