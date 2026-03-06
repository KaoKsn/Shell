#ifndef BUILT_INS_C
#define BUILT_INS_C

#include "../include/built_ins.h"

// Check if cmd is a shell built-in.
int builtin(char *cmd)
{
    if (cmd) {
        int b = 0, e = SUPPORTED_CMDS - 1;
        while (b <= e) {
            int mid = (b + e) / 2;
            int cmpval = strcmp(ARGS_LIST[mid], cmd);
            if (cmpval == 0)
                return mid;
            else if (cmpval < 0)
                b = mid + 1;
            else
                e = mid - 1;
        }
    }
    return -1;
}

// Concatenate the contents of text file(s).
int cat (char **files, int tfiles)
{
    if (files == NULL)
        return 0;
    for (int i = 0; i < tfiles; i++) {
        FILE *fptr = fopen(files[i], "r");
        if (fptr == NULL) {
            perror("");
            continue;
        }
        int c;
        while ((c = fgetc(fptr)) != EOF) {
            printf("%c", c);
        }
        fclose(fptr);
    }
    printf("\n");
    return 0;
}

// Print the current date set by locale.
int date()
{
    time_t tm = time(NULL);
    printf("%s", ctime(&tm));
    return 0;
}

// Check if cmd is a built-in/executable in PATH.
int type(char *cmd)
{
    if (builtin(cmd) != -1)
        printf("%s: is a shell built-in\n", cmd);
    else
        printf("%s: type unknown\n", cmd);
    return 0;
}
#endif /* ifndef BUILT_INS_C */
