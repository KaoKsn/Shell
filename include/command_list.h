#ifndef COMMAND_LIST_H
#define COMMAND_LIST_H

char *PATH = "/usr/local/sbin:\
              /usr/local/bin:\
              /usr/bin:\
              /usr/lib/jvm/default/bin:\
              /usr/bin/site_perl:\
              /usr/bin/vendor_perl:/usr/bin/core_perl";

#define SUPPORTED_CMDS 6

enum COMMAND_IDS {
    CAT,
    DATE,
    ECHO,
    EXIT,
    PWD,
    TYPE,
};
char *ARGS_LIST[] = {
    "cat",
    "date",
    "echo",
    "exit",
    "pwd",
    "type",
};
#endif
