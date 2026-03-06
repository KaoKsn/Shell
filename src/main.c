#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/command_list.h"
#include "./built_ins.c"

#define MAX_ARGLEN 128
#define MAX_ARGS 16
#define INPUT_BUFFER_SIZE MAX_ARGLEN * MAX_ARGS + MAX_ARGS - 1
#define INVALID_INPUT 1

int read_input(char *cmd);
char **breakdown(char *cmd, int len, int *targs);
void printargs(char **args, int targs);
int execute(int cmd_id, char **cmdargs, int targs);
void freecmdargs(char **args);

static void sigint_handler(int s) 
{
    printf("\nBye\n");
    exit(0);
}
int main(void) {
    signal(SIGINT, sigint_handler);
    int targs = 0, exit_status, input_status, cmd_id;
    char cmd[INPUT_BUFFER_SIZE];
    while (true) {
        setbuf(stdout, NULL);
        printf("$ ");

        memset(cmd, 0, sizeof(cmd));

        input_status = read_input(cmd);
        if (input_status != 0)
            continue;
        char **cmdargs = breakdown(cmd, strlen(cmd), &targs);
        if (cmdargs == NULL) {
            fprintf(stderr, "Allocation Failed!\n");
            continue;
        }
        char *bin = cmdargs[0];
        cmd_id = builtin(bin);
        exit_status = execute(cmd_id, cmdargs, targs);
        if (exit_status == -1)
            fprintf(stderr, "%s: command not found\n", bin);
        else if (exit_status > 0)
            fprintf(stderr, "%s: Failed (exec status: %d).\n", bin, exit_status);
        freecmdargs(cmdargs);
    }
    return 0;
}

int read_input(char *cmd)
{
    if (fgets(cmd, sizeof(char) * INPUT_BUFFER_SIZE, stdin) == NULL) {
        putchar('\n');
        return INVALID_INPUT;
    }
    int len = strlen(cmd);
    if (cmd[len - 1] == '\n') {
        cmd[len - 1] = '\0';
    } else {  // Empty buffer on overflow.
        int c;
        while ((c = getchar()) != EOF && c != '\n');
        return INVALID_INPUT;
    }
    // Empty input.
    if (cmd[0] == '\0')
        return INVALID_INPUT;
    return 0;
}

char **breakdown(char *cmd, int len, int *targs)
{
    char **cmd_args = calloc(MAX_ARGS, sizeof(char *));
    if (cmd_args == NULL) {
            return NULL;
    }
    for (int i = 0; i < MAX_ARGS; i++) {
        cmd_args[i] = calloc(MAX_ARGLEN + 1, sizeof(char));
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
            fprintf(stderr, "\nOnly 16 arguments(including name) allowed!\n");
            printf("Going ahead with...\n\t");
            printargs(cmd_args, *targs);
            printf("\n");
            return cmd_args;
        }
        // TODO: Add support to format: "strings how".
        char c = cmd[i];
        if (c == ' ' || c == '\0') {
            // Allow only MAX_ARGLEN.
            if (arglen > MAX_ARGLEN) {
                fprintf(stderr, "\n\tMax argument length allowed: %d\n", MAX_ARGLEN);
                fprintf(stderr, "\tTruncating others!\n\n");
                arglen = MAX_ARGLEN;
            }
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

void freecmdargs(char **cmdargs)
{
    if (cmdargs) {
        for (int i = 0; i < MAX_ARGS; i++)
            free(cmdargs[i]);
        free(cmdargs);
    }
}

void printargs(char **args, int targs)
{
    if (args) {
        for (int i = 0; i < targs; i++)
            printf("%s ", args[i]);
    }
    printf("\n");
}


int execute(int cmd_id, char **cmdargs, int targs)
{
    if (cmdargs) {
        switch (cmd_id) {
            case CAT:
                    return cat(cmdargs + 1, targs - 1);
            case DATE:
                    return date();
            case ECHO:
                    printargs(cmdargs + 1, targs - 1);
                    return 0;
            case EXIT:
                    printf("Bye...\n");
                    freecmdargs(cmdargs);
                    exit(0);
            case TYPE:
                if (targs < 2)
                    return 1;
                char *cmd = cmdargs[1];
                return type(cmd);
            default:
                return -1;
        }
    }
    return -1;
}
