#ifndef COMMAND_LIST_H
#define COMMAND_LIST_H

extern char *PATH;
extern char *ARGS_LIST[];

#ifndef SUPPORTED_CMDS
#define SUPPORTED_CMDS 6
#endif

enum COMMAND_IDS {
    CAT,
    DATE,
    ECHO,
    EXIT,
    PWD,
    TYPE,
};
#endif // COMMAND_LIST_H
