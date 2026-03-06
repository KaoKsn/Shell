#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ARGLEN 64
#define MAX_ARGS 16
#define INPUT_BUFFER_SIZE MAX_ARGLEN * MAX_ARGS + MAX_ARGS - 1

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
    }
    return 0;
}
