#include "../include/built_ins.h"
#include "../include/shell.h"
#include "../include/path.h"

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

// Change the current working directory.
int cd(char *path)
{
    if (path) {
        char *fpath = path, *abspath = NULL;
        // Only substitue the first ~, if it exists.
        if (path[0] == '~') {
            char *homedir = getenv("HOME");
            if (homedir == NULL) {
                fprintf(stderr, "Variable HOME not set!\n");
                return EXIT_FAILURE;
            }
            abspath = calloc(PATH_MAX + 1, sizeof(char));
            if (abspath == NULL) {
                // If calloc fails, use the stack.
                char sabspath[PATH_MAX + 1] = {'\0'};
                fpath = sabspath;
                strncat(sabspath, homedir, PATH_MAX - 1);
                if (strlen(path) > 1)
                    strncat(sabspath, path + 1, PATH_MAX - 1 - strlen(sabspath));
            } else {
                fpath = abspath;
                strncat(abspath, homedir, PATH_MAX - 1);
                // Copy anything after the beginning ~ as is.
                if (strlen(path) > 1)
                    strncat(abspath, path + 1, PATH_MAX - 1 - strlen(abspath));
            }
        }
        // Perform chdir on the abspath obtained.
        if (chdir(fpath) != 0) {
            fprintf(stderr, "cd: %s", fpath);
            perror(" ");
            if (fpath == abspath)
                free(abspath);
            return EXIT_FAILURE;
        }
        if (fpath == abspath)
            free(abspath);
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}

// Copy raw bytes from src to dest.
int dd(char *src, char *dest)
{
    FILE *fsrc = fopen(src, "rb");
    if (fsrc == NULL) {
        fprintf(stderr, "dd: %s: ", src);
        perror(NULL);
        return 1;
    }
    FILE *fdest = fopen(dest, "wb");
    if (fdest == NULL) {
        perror("dd: fopen");
        return 2;
    }
    uint8_t buffer;
    while (fread(&buffer, sizeof(char), 1, fsrc) == 1) {
        if (fwrite(&buffer, sizeof(char), 1, fdest) != 1) {
            perror("fwrite");
            break;
        }
    }
    fclose(fdest);
    // Check if read was successful.
    if (feof(fsrc) == 0) {
        fprintf(stderr, "Write unsuccessful!\n");
        fclose(fsrc);
        return 3;
    }
    fclose(fsrc);
    return 0;
}

// Remove empty directories.
int _rmdir(char **dirs, int tdirs)
{
    int rval = 1;
    for (int i = 0; i < tdirs; i++) {
        if (rmdir(dirs[i]) != 0) {
            fprintf(stderr, "%s: ", dirs[i]);
            perror("");
            rval = 1;
        }
    }
    return rval;
}
// Print the current date set by locale.
int date()
{
    time_t tm = time(NULL);
    printf("%s", ctime(&tm));
    return 0;
}

// Check if cmd is a built-in/executable in PATH.
int type(PATH_t *path, char *cmd)
{
    char *loc;
    if (builtin(cmd) != -1) {
        printf("%s: is a shell builtin\n", cmd);
    } else if ((loc = find_in_path(path, cmd)) != NULL) {
        printf("%s is %s\n", cmd, loc);
        free(loc);
    } else {
        printf("%s: not found\n", cmd);
    }
    return 0;
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

int help()
{
    printf("You have the following shell built-ins at your disposal:\n");
    for (int i = 0; i < SUPPORTED_CMDS; i++)
        printf("\t%s\n", ARGS_LIST[i]);
    return 0;
}

int nslookup(char *domain)
{
    if (domain == NULL) {
        fprintf(stderr, "domain is NULL\n");
        return 1;
    }
    struct addrinfo hints, *res;
    int status;
    char *ip;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    status = getaddrinfo(domain, NULL, &hints, &res);
    if (status != 0) {
        fprintf(stderr, "nslookup: %s\n", gai_strerror(status));
        return 1;
    }
    printf("%s:\n", domain);

    ip = calloc(INET6_ADDRSTRLEN, sizeof(char));
    if (ip == NULL) {
        // Use stack allocation.
        char ipstr[INET6_ADDRSTRLEN] = {'\0'};
        return get_ips(res, ipstr);
    }
    status = get_ips(res, ip);
    free(ip);
    return status;
}

int get_ips(struct addrinfo *res, char *ipstr)
{
    if (res == NULL || ipstr == NULL) {
        fprintf(stderr, "address list empty | ipstr empty!\n");
        if (res)
            freeaddrinfo(res);
        return 1;
    }
    for (struct addrinfo *p = res; p != NULL; p = p->ai_next) {
        memset(ipstr, 0, INET6_ADDRSTRLEN);
        if (p->ai_family == AF_INET) {  // ipv4
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
            inet_ntop(p->ai_family, &(ipv4->sin_addr), ipstr, INET6_ADDRSTRLEN);
        } else {
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
            inet_ntop(p->ai_family, &(ipv6->sin6_addr), ipstr, INET6_ADDRSTRLEN);
        }
        printf("\t%s\n", ipstr);
    }
    freeaddrinfo(res);
    return 0;
}

// Spawn a child.
int try_exec(char *bin, char **cmdargs, int targs)
{
    if (bin == NULL)
        return -1;
    pid_t child = fork();
    if (child < 0) {
        perror("fork");
        free(bin);
        return 1;
    } else if (child == 0) {
        char **argv = calloc(targs + 1, sizeof(char *));
        if (argv == NULL) {
            fprintf(stderr, "Setting argv failed!\n");
            free(bin);
            freecmdargs(cmdargs);
            exit(-1);
        }
        for (int i = 0; i < targs; i++)
            argv[i] = cmdargs[i];
        argv[targs] = NULL;

        execvp(bin, argv);
        if (errno == -1) {
            perror("execvpe");
        }
        freecmdargs(cmdargs);
        free(bin);
        free(argv);
        exit(1);    // Child failed to exec.
    } else {
        int r_wait = wait(NULL);
        free(bin);
        if (r_wait == child) {
            return 0;
        } else {
            perror("wait");
            return r_wait;
        }
    }
}
