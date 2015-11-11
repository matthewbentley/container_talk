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
#include <sys/syscall.h>

static int child(void *arg) {
    arg = (void *) arg;
    printf("My PID: %ld\n", (long) getpid());
    printf("My PPID: %ld\n", (long) getppid());

    mount("", "/", "dontcare", MS_REC|MS_SLAVE, NULL);
    chroot("/opt/debian");
    mount("proc", "/proc", "proc", 0, NULL);
    chdir("/");

    execlp("/bin/bash", "/bin/bash", (char *) NULL);
    return 0;
}

#define STACK_SIZE (1024 * 1024)

static char child_stack[STACK_SIZE];

int main(int argc, char *argv[]) {
    pid_t child_pid;

    if (argc < 1)
        return 1;

    child_pid = clone(
            child,
            child_stack + STACK_SIZE,
            CLONE_NEWNS | SIGCHLD, argv[1]);

    printf("PID of child: %ld\n", (long) child_pid);

    waitpid(child_pid, NULL, 0);

    return 0;
}
