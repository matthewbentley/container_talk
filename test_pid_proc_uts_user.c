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
#include <string.h>
#include <fcntl.h>

void update_map(int in, int out, int len, pid_t pid) {
    int fd;
    size_t map_len;
    char *mapping;
    char *map_file;

    map_len = snprintf(NULL, 0, "%d %d %d", in, out, len) + 1;
    mapping = malloc(map_len);
    sprintf(mapping, "%d %d %d", in, out, len);

    map_file = malloc(snprintf(NULL, 0, "/proc/%ld/uid_map", (long) pid) + 1);
    sprintf(map_file, "/proc/%ld/uid_map", (long) pid);

    fd = open(map_file, O_RDWR);

    write(fd, mapping, map_len);

    close(fd);
    free(mapping);
    free(map_file);
}

static int child(void *arg) {
    char *newhostname = arg;

    sethostname(newhostname, strlen(newhostname));

    printf("My PID: %ld\n", (long) getpid());
    printf("My PPID: %ld\n", (long) getppid());
        printf("eUID = %ld;  eGID = %ld\n;  ",
                (long) geteuid(), (long) getegid());

    mount("", "/", "dontcare", MS_REC|MS_PRIVATE, NULL);

    umount("/proc");
    mount("proc", "/proc", "proc", 0, NULL);

    execlp("chroot", "chroot", "/opt/debian", "/bin/bash", (char *) NULL);
    return 0;
}

#define STACK_SIZE (1024 * 1024)

static char child_stack[STACK_SIZE];

int main(int argc, char *argv[]) {
    pid_t child_pid;

    if (argc < 2)
        return 1;

    child_pid = clone(
            child,
            child_stack + STACK_SIZE,
            CLONE_NEWNS | CLONE_NEWPID | CLONE_NEWUTS | CLONE_NEWUSER | SIGCHLD,
            argv[1]);

    printf("PID of child: %ld\n", (long) child_pid);
    update_map(0, 1000, 1, child_pid);

    waitpid(child_pid, NULL, 0);

    return 0;
}
