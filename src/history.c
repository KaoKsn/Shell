#include "../include/history.h"

int hist_init()
{
    int fd;
    char *home_dir = getenv("HOME");
    if (home_dir == NULL)
        return HOME_NOT_FOUND;
    char *hist_dir = calloc(PATH_MAX + 1, sizeof(char));
    if (hist_dir == NULL) {
        // Use a static array of chars.
        char hist_dir_stat[PATH_MAX + 1] = {'\0'};
        strncpy(hist_dir_stat, home_dir, PATH_MAX);
        strncat(hist_dir_stat, "/.sh_history", PATH_MAX - strlen(hist_dir_stat));

        // Open shell history file to document command history.
        fd = open(hist_dir_stat, O_CREAT | O_WRONLY | O_APPEND , 0644);
        if (fd <= 0)
            perror("open");
        return fd;
    }

    strncpy(hist_dir, home_dir, PATH_MAX);
    strncat(hist_dir, "/.sh_history", PATH_MAX - strlen(hist_dir));

    // Open shell history file to document command history.
    fd = open(hist_dir, O_CREAT | O_WRONLY | O_APPEND , 0644);
    if (fd <= 0)
        perror("open");
    free(hist_dir);
    return fd;
}
// Append a command into the history file.
bool append(int fd, char *cmd)
{
    if (cmd) {
        time_t tm = time(NULL);
        char *curtime = ctime(&tm);
        // Remove the trailing newline.
        strtok(curtime, "\n");

        char buffer[HIST_BUFF_SIZE] = {'\0'};
        strncat(buffer, curtime, 32);
        strncat(buffer, " ", HIST_BUFF_SIZE - strlen(buffer));
        strncat(buffer, cmd, HIST_BUFF_SIZE - strlen(buffer));
        strncat(buffer, "\n", HIST_BUFF_SIZE - strlen(buffer));

        if (write(fd, buffer, HIST_BUFF_SIZE) != HIST_BUFF_SIZE) {
            perror("write");
            return false;
        }
        return true;
    }
    return false;
}
