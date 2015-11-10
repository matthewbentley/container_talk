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

static int child(void *arg) {
    char *newhostname = arg;

    sethostname(newhostname, strlen(newhostname));

    printf("My PID: %ld\n", (long) getpid());
    printf("My PPID: %ld\n", (long) getppid());

    mount("", "/", "dontcare", MS_REC|MS_SLAVE|MS_BIND, NULL);

    umount("/proc");
    mount("proc", "/proc", "proc", 0, NULL);

    execlp("bash", "bash", (char *) NULL);
    return 0;
}

#define STACK_SIZE (1024 * 1024)

static char child_stack[STACK_SIZE];

int main(int argc, char *argv[]) {
    pid_t child_pid;
    int fd;

    if (argc < 3)
        return 1;

    fd = open(argv[2], O_RDONLY);
    setns(fd, 0);

    child_pid = clone(
            child,
            child_stack + STACK_SIZE,
            CLONE_NEWNS | CLONE_NEWPID | CLONE_NEWUTS | SIGCHLD, argv[1]);

    printf("PID of child: %ld\n", (long) child_pid);

    waitpid(child_pid, NULL, 0);

    return 0;
}
