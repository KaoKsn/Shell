#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

#include "./shell.h"

#ifndef HOME_NOT_FOUND
#define HOME_NOT_FOUND -1
#endif

#ifndef HIST_INIT_FAILED
#define HIST_INIT_FAILED 1
#endif

#ifndef HIST_BUFF_SIZE
#define HIST_BUFF_SIZE INPUT_BUFFER_SIZE + 128
#endif

int hist_init();
bool append(int fd, char *cmd);
