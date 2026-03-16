#include "../include/util.h"

char **split(char *str, char *delim, int *tdirs)
{
    if (str && delim) {
        int buffsize = 64, pos = 0;
        char **tokens = calloc(buffsize, sizeof(char *));
        char *token;

        if (tokens == NULL)
            return NULL;

        token = strtok(str, delim);
        while (token != NULL) {
            tokens[pos++] = token;
            if (pos >= buffsize) {
                buffsize *= 2;
                char **tmp = realloc(tokens, buffsize * sizeof(char *));
                if (tmp == NULL) {
                    fprintf(stderr, "Realloc Failure!\n");
                    free(tokens);
                    return NULL;
                }
                tokens = tmp;
            }
            token = strtok(NULL, delim);
        }
        // NULL terminate.
        tokens[pos] = NULL;
        *tdirs = pos;
        return tokens;
    }
    return NULL;
}
