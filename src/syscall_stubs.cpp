#include <unistd.h>
#include <sys/stat.h>
#include <sys/select.h>

extern "C" {
    int lstat(const char *path, struct stat *buf) { return 0; };
    int pipe(int fildes[2]) { return 0; };
    int execl(const char *path, const char *arg0, ... /*, (char *)0 */) { return -1; };
    int dup(int oldfd) { return 0; };
    int dup2(int oldfd, int newfd) { return 0; };
    pid_t waitpid(pid_t pid, int *status, int options) { return 1; };
    int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout) { return 0; };
    mode_t umask(mode_t mask) { return S_IWGRP | S_IWOTH; };
    char *getcwd(char *buf, size_t size) { return nullptr; };
    int flock(int fd, int operation) { return 0; };
    int symlink(const char *oldpath, const char *newpath) { return 0; };
    int chmod(const char *path, mode_t mode) { return 0; };
    ssize_t readlink(const char *path, char *buf, size_t bufsiz) { return 0; };
    char *basename(char *path) { return nullptr; };
    char *dirname(char *path) { return nullptr; };
    char *realpath(const char *path, char *resolved_path) { return nullptr; };
    struct passwd *getpwnam(const char *name) { return nullptr; };
}