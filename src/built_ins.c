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
    else if (isexecutable(cmd) == false)
        printf("%s: not found\n", cmd);
    return 0;
}

// Look dir for file.
int search_in(char *dir, char *file) {
    if (dir) {
        strcat(dir, file);
        struct stat sb;
        if (stat(dir, &sb) == 0 && (sb.st_mode & S_IEXEC))
            return 1;
        else
            // File found with no execute perm(0).
            return 0;
    }
    return 0;
}

// Check if a given binary is present in PATH.
bool isexecutable(char *cmd) {
    char delimiter = strstr(PATH, ":") ? ':' : ';';
    char *dir = calloc(PATH_MAX, sizeof(char));
    for (size_t i = 0, k = 0; PATH[i] != '\0'; i++) {
        if (PATH[i] == delimiter || i == strlen(PATH) - 1) {
            if (i == strlen(PATH) - 1)
                dir[k++] = PATH[i];
            if (dir[strlen(dir)-1] != '/')
                strcat(dir, "/");
            if (search_in(dir, cmd) == 1) {
                printf("%s is %s\n", cmd, dir);
                free(dir);
                return true;
            }
            // Check in the next path.
            memset(dir, 0, PATH_MAX * sizeof(char));
            k = 0;
        } else {
            dir[k++] = PATH[i];
        }
    }
    free(dir);
    return false;
}
#endif /* ifndef BUILT_INS_C */
