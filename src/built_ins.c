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
