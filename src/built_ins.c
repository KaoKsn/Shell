#ifndef BUILT_INS_C
#define BUILT_INS_C

#include "../include/built_ins.h"

int cat (char **files, int tfiles)
{
    if (files == NULL)
        return 0;
    for (int i = 0; i < tfiles; i++) {
        printf("%s\n", files[i]);
        FILE *fptr = fopen(files[i], "r");
        if (fptr == NULL) {
            perror("");
            continue;
        }
        int c;
        while ((c = fgetc(fptr)) != EOF) {
            printf("%c", c);
        }
        printf("\n");
        fclose(fptr);
    }
    return 0;
}

int date()
{
    time_t tm = time(NULL);
    printf("%s", ctime(&tm));
    return 0;
}

int type(char *cmd)
{
    if (is_valid(cmd) != -1)
        printf("%s: is a shell built-in\n", cmd);
    else
        printf("%s: type unknown\n", cmd);
    return 0;
}
#endif /* ifndef BUILT_INS_C */
