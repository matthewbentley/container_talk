#define _GNU_SOURCE
#include <sys/utsname.h>
#include <sys/wait.h>
#include <stdio.h>
#include <sched.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

static int child(void *arg) {
    arg = (void *) arg;

    execlp("bash", "bash", (char *) NULL);

    return 0;
}

#define STACK_SIZE (1024 * 1024)
static char child_stack[STACK_SIZE];

int main(int argc, char *argv[]) {
    pid_t child_pid;

    if (argc < 2)
        return 1;

    if (chroot(argv[1]))
        printf("Oops\n");


    execlp("bash", "bash", (char *) NULL);

    //child_pid = clone(child,
    //        child_stack + STACK_SIZE,
    //        SIGCHLD,
    //        argv[1]
    //);

    //printf("PID of child process is %ld\n", (long) child_pid);

    //printf("PID of this program is %ld\n", (long) getpid());

    //waitpid(child_pid, NULL, 0);

    return 0;
}
