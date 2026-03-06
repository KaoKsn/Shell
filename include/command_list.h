#ifndef COMMAND_LIST_H
#define COMMAND_LIST_H

#define SUPPORTED_CMDS 6

enum COMMAND_IDS {
    CAT,
    DATE,
    ECHO,
    EXIT,
    PRINT_ARGS,
    TYPE,
};
char *ARGS_LIST[] = {
    "cat",
    "date",
    "echo",
    "exit",
    "printargs",
    "type",
};
#endif
