#ifndef COMMAND_LIST_H
#define COMMAND_LIST_H

extern char *PATH;
extern char *ARGS_LIST[];

#ifndef SUPPORTED_CMDS
#define SUPPORTED_CMDS 10
#endif

enum COMMAND_IDS {
    CAT,
    DATE,
    DD,
    ECHO,
    EXIT,
    HELP,
    HOSTNAME,
    NSLOOKUP,
    PWD,
    TYPE,
};
#endif // COMMAND_LIST_H
