#ifndef BUILT_INS_H
#define BUILT_INS_H

#include <arpa/inet.h>
#include <errno.h>
#include <linux/limits.h>
#include <netdb.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#include "./command_list.h"

#define OPEN_ERR 2
#define HOST_BUFF_MAX 256

bool isexecutable(char *cmd);
int builtin(char *cmd);
int get_ips(struct addrinfo *res, char *ipstr);
int search_in(char *dir, char *file);

int cat(char **files, int tfiles);
int date();
int dd(char *src, char *dest);
int help();
int hostname();
int nslookup(char *domain);
int pwd();
int _rmdir(char **dirs, int tdirs);
int type(char *cmd);

#endif // !BUILT_INS_H
