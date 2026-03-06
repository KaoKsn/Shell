#ifndef COMMAND_LIST_H
#define COMMAND_LIST_H

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
