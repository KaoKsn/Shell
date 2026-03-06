#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ARGLEN 64
#define MAX_ARGS 16
#define INPUT_BUFFER_SIZE MAX_ARGLEN * MAX_ARGS + MAX_ARGS - 1

char **breakdown(char *cmd, int len, int *targs);
void freecmdbuffer(char **args);

int main(void) {
    int targs = 0;
    char cmd[INPUT_BUFFER_SIZE];
    while (true) {
        setbuf(stdout, NULL);
        printf("$ ");

        memset(cmd, 0, sizeof(cmd));
        if (fgets(cmd, sizeof(char) * INPUT_BUFFER_SIZE, stdin) == NULL) {
            putchar('\n');
            continue;
        }

        if (cmd[strlen(cmd) - 1] == '\n') {
            cmd[strlen(cmd) - 1] = '\0';
        } else {  // Empty stdout on overflow.
            int c;
            while ((c = getchar()) != EOF && c != '\n');
            continue;
        }
        char **cmdbuffer = breakdown(cmd, strlen(cmd), &targs);
        if (cmdbuffer == NULL) {
            fprintf(stderr, "Allocation Failed!\n");
            continue;
        }
        freecmdbuffer(cmdbuffer);
    }
    return 0;
}

char **breakdown(char *cmd, int len, int *targs)
{
    char **cmd_args = calloc(MAX_ARGS, sizeof(char *));
    if (cmd_args == NULL) {
            return NULL;
    }
    for (int i = 0; i < MAX_ARGS; i++) {
        cmd_args[i] = calloc(MAX_ARGLEN, sizeof(char));
        if (cmd_args[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(cmd_args[i]);
            }
            free(cmd_args);
            return NULL;
        }
    }
    *targs = 0;
    int arglen = 0, argstart_indx = 0;
    for (int i = 0; i <= len; i++) {
        // Allow only MAX_ARGS.
        if (*targs == MAX_ARGS) {
            fprintf(stderr, "Only 16 arguments(including name) allowed!\n");
            return cmd_args;
        }
        // TODO: Add support to format: "strings how".
        char c = cmd[i];
        if (c == ' ' || c == '\0') {
            strncpy(cmd_args[*targs], cmd + argstart_indx, arglen);
            cmd_args[(*targs)++][arglen] = '\0';
            // Setup for the next arg read.
            argstart_indx = i + 1, arglen = 0;
            continue;
        }
        arglen++;
    }
    return cmd_args;
}

void freecmdbuffer(char **cmdbuffer)
{
    if (cmdbuffer) {
        for (int i = 0; i < MAX_ARGS; i++)
            free(cmdbuffer[i]);
        free(cmdbuffer);
    }
}
