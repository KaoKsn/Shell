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
            fprintf(stderr, "cat: %s", files[i]);
            perror(" ");
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
    char dir[PATH_MAX] = {'\0'};
    for (size_t i = 0, k = 0; PATH[i] != '\0'; i++) {
        if (PATH[i] == delimiter || i == strlen(PATH) - 1) {
            if (i == strlen(PATH) - 1)
                dir[k++] = PATH[i];
            if (dir[strlen(dir)-1] != '/')
                strcat(dir, "/");
            if (search_in(dir, cmd) == 1) {
                printf("%s is %s\n", cmd, dir);
                return true;
            }
            // Check in the next path.
            memset(dir, 0, PATH_MAX * sizeof(char));
            k = 0;
        } else {
            dir[k++] = PATH[i];
        }
    }
    return false;
}

int pwd()
{
    char *path = calloc(PATH_MAX, sizeof(char));
    if (path == NULL) {
        char pathbuff[PATH_MAX] = {'\0'};
        if (getcwd(pathbuff, sizeof(PATH_MAX)) != NULL) {
            printf("%s\n", pathbuff);
            return 0;
        } else {
            return 1;
        }
    } else if (getcwd(path, PATH_MAX) != NULL){
        printf("%s\n", path);
        free(path);
        return 0;
    }
    free(path);
    return ERANGE;
}

int hostname()
{
    char *buffer = calloc(HOST_BUFF_MAX, sizeof(char));
    if (buffer == NULL) {
        char buff[HOST_BUFF_MAX] = {'\0'};
        if (gethostname(buff, sizeof(char) * HOST_BUFF_MAX) == 0) {
            printf("%s\n", buff);
            return 0;
        }
        else {
            perror("hostname:");
            return 1;
        }
    } else {
        if (gethostname(buffer, sizeof(char) * HOST_BUFF_MAX) == 0) {
            printf("%s\n", buffer);
            free(buffer);
            return 0;
        }
        else {
            perror("hostname:");
            free(buffer);
            return 1;
        }
    }
    return 0;
}
