#ifndef COMMAND_LIST_H
#define COMMAND_LIST_H

char *PATH = "/usr/local/sbin:/usr/local/bin:/usr/bin:/usr/lib/jvm/default/bin:/usr/bin/site_perl:/usr/bin/vendor_perl:/usr/bin/core_perl";

#define SUPPORTED_CMDS 5

enum COMMAND_IDS {
    CAT,
    DATE,
    ECHO,
    EXIT,
    TYPE,
};
char *ARGS_LIST[] = {
    "cat",
    "date",
    "echo",
    "exit",
    "type",
};
#endif
