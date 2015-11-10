#define _GNU_SOURCE
#include <sched.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/mount.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <signal.h>
#include <stdio.h>
#include <fcntl.h>

struct child_args {
    char *hostname;
    char *chroot_to;
};

static int child(void *arg) {
    struct child_args *cargs = (struct child_args *) arg;

    sethostname(cargs->hostname, strlen(cargs->hostname));

    printf("My PID: %ld\n", (long) getpid());
    printf("My PPID: %ld\n", (long) getppid());

    mount("", "/", "dontcare", MS_REC|MS_SLAVE, NULL);
    chroot(cargs->chroot_to);
    umount("/proc");
    mount("proc", "/proc", "proc", 0, NULL);
    chdir("/");

    execlp("/bin/bash", "/bin/bash", (char *) NULL);

    return 0;
}

#define STACK_SIZE (1024 * 1024)

static char child_stack[STACK_SIZE];

int main(int argc, char *argv[]) {
    pid_t child_pid;
    int fd;
    struct child_args cargs;


    if (argc < 4)
        return 1;

    cargs.hostname = argv[1];
    cargs.chroot_to = argv[3];

    fd = open(argv[2], O_RDONLY);
    setns(fd, 0);

    child_pid = clone(
            child,
            child_stack + STACK_SIZE,
            CLONE_NEWNS | CLONE_NEWPID | CLONE_NEWUTS | SIGCHLD, &cargs);

    printf("PID of child: %ld\n", (long) child_pid);

    waitpid(child_pid, NULL, 0);

    return 0;
}
