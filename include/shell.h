#ifndef SHELL_H
#define SHELL_H

#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "./command_list.h"
#include "./path.h"

#define INVALID_INPUT 1
#define MAX_ARGLEN 128
#define MAX_ARGS 16
#define INPUT_BUFFER_SIZE MAX_ARGLEN * MAX_ARGS + MAX_ARGS - 1

int read_input(char *cmd);
char **breakdown(char *cmd, int len, int *targs);
void printargs(char **args, int targs);
int execute(int cmd_id, char **cmdargs, int targs, PATH_t *path);
void freecmdargs(char **args);

#endif // !SHELL_H

