#define _GNU_SOURCE
#include <sys/utsname.h>
#include <sys/wait.h>
#include <stdio.h>
#include <sched.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static int child(void *arg) {
    struct utsname uts;
    sethostname(arg, strlen(arg));

    uname(&uts);

    printf("uts.nodename in child: %s\n", uts.nodename);

    sleep(10000);

    return 0;
}

#define STACK_SIZE (1024 * 1024)
static char child_stack[STACK_SIZE];

int main(int argc, char *argv[]) {
    struct utsname uts;
    int child_pid;

    if (argc < 2)
        return 1;

    child_pid = clone(child,
            child_stack + STACK_SIZE,
            CLONE_NEWUTS | SIGCHLD,
            argv[1]
    );

    printf("PID of child process is %ld\n", (long) child_pid);
    sleep(1);

    uname(&uts);

    printf("PID of this program is %ld\n", (long) getpid());
    printf("uts.namename: %s\n", uts.nodename);

    waitpid(child_pid, NULL, 0);

    return 0;
}
