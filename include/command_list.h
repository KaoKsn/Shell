#ifndef COMMAND_LIST_H
#define COMMAND_LIST_H

extern char *ARGS_LIST[];

#ifndef SUPPORTED_CMDS
#define SUPPORTED_CMDS 13
#endif

enum COMMAND_IDS {
    CAT,
    CD,
    DATE,
    DD,
    ECHO,
    EXIT,
    HELP,
    HOSTNAME,
    MKDIR,
    NSLOOKUP,
    PWD,
    RMDIR,
    TYPE,
};
#endif // COMMAND_LIST_H
