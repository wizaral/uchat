#include "server.h"

static void err_quit(const char *s, char *cmd) {
    printf(s, cmd);
    exit(EXIT_FAILURE);
}
/*
 * Ensure the impossibility of gaining a control terminal in the future.
 * Set the root directory as the current working directory,
 * so that you can later unmount the file system.
 */
static pid_t insurance(char *cmd, pid_t pid) {
    struct sigaction sa;

    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGHUP, &sa, NULL) < 0)
        err_quit("%s: immposible to ignore signal SIGHUP", cmd);
    if ((pid = fork()) < 0)
        err_quit("%s: error fork", cmd);
    else if (pid != 0) // Close parent process.
        exit(EXIT_SUCCESS);
    if (chdir("/") < 0)
        err_quit("%s: impossible to make current working directory /", cmd);
    return getpid();
}
/*
 * Close all open file descriptors.
 * Attach file descriptors 0, 1, and 2 to /dev/null.
 * Initialize the log file.
 */
static void manage_fd(char *cmd, struct rlimit *rl) {
    int fd0;
    int fd1;
    int fd2;

    if (rl->rlim_max == RLIM_INFINITY)
        rl->rlim_max = 1024;
    for (rlim_t i = 0; i < rl->rlim_max; i++)
        close(i);
    fd0 = open("/dev/null", O_RDWR);
    fd1 = dup(0);
    fd2 = dup(0);
    openlog(cmd, LOG_CONS, LOG_DAEMON);
    if (fd0 != 0 || fd1 != 1 || fd2 != 2) {
        syslog(LOG_ERR, "error file descriptors %d %d %d", fd0, fd1, fd2);
        exit(EXIT_FAILURE);
    }
}

void mx_daemonize(void) {
    pid_t pid;
    pid_t sed;
    struct rlimit rl;
    char *cmd = "./uchat_server";

    umask(0);  // Reset file creation mode mask.
    if (getrlimit(RLIMIT_NOFILE, &rl) < 0)  // Get max number file descriptor.
        err_quit("%s: impossible to get max number file descriptor", cmd);
    if ((pid = fork()) < 0)  // Become the leader to lose the managem. termin.
        err_quit("%s: error fork", cmd);
    else if (pid != 0)  // Close parent process.
        exit(EXIT_SUCCESS);
    if ((sed = setsid()) < 0)
        exit(EXIT_FAILURE);
    pid = insurance(cmd, pid);
    printf("server id -> %d\n", pid);
    manage_fd(cmd, (struct rlimit *)&rl);
}
