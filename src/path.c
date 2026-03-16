#include "../include/path.h"
#include "../include/util.h"

PATH *path = NULL;

// Load the users PATH on shell init.
PATH *load_path()
{
    PATH *path = malloc(sizeof(PATH));
    if (path == NULL) {
        fprintf(stderr, "*** Malloc for env PATH failed!\n");
        fprintf(stderr, "\t*** Binaries requiring it might not work as expected!\n");
        return NULL;
    }
    path->tdirs = 0;
    char *path_env = getenv("PATH");
    if (path_env == NULL) {
        fprintf(stderr, "*** env PATH is EMPTY! ***\n");
        fprintf(stderr, "\t*** Binaries present in/requiring it might not work as expected!\n");
        return NULL;
    }
    char *delim = strstr(path_env, ":") ? ":" : ";";
    path->dirs = split(path_env, delim, &(path->tdirs));
    if (path->dirs == NULL) {
        fprintf(stderr, "Split Failed!\n");
        free(path);
        return NULL;
    }
    return path;
}

// Return the absolute path to a binary if it exists in the PATH.
char *find_in_path(PATH *path, char *target)
{
    if (path && target) {
        struct stat sb;

        for (size_t i = 0; path->dirs[i] != NULL; i++) {
            if (path->dirs[i] == NULL)
                continue;

            char *abspath = calloc(PATH_MAX + 1, sizeof(char));
            if (abspath == NULL)
                return NULL;
            strncat(abspath, path->dirs[i], PATH_MAX);
            strncat(abspath, "/", PATH_MAX - strlen(abspath));
            strncat(abspath, target, PATH_MAX - strlen(abspath));

            // If the binary exists with exec perms available.
            if ((stat(abspath, &sb) == 0) && (sb.st_mode & S_IEXEC))
                return abspath;
            // Continue to the next dir.
            free(abspath);
        }
        return NULL;
    }
    return NULL;
}

void freepath(PATH *path)
{
    if (path) {
        free(path->dirs);
        free(path);
    }
}
